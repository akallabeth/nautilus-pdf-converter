/**
 * Copyright (c) 2021 Armin Novak, Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 */
#include <sstream>
extern "C" {
#include "nautilus-pdf-converter.h"
#include <nautilus-extension.h>
}
static GType pdf_converter_type = 0;

static void nautilus_pdf_converter_instance_init(GTypeInstance *instance,
                                                 gpointer g_class) {
  auto img = reinterpret_cast<NautilusPdfConverter *>(g_class);
}

static void nautilus_pdf_converter_class_init(gpointer g_class,
                                              gpointer class_data) {
}

static bool pdf_converter_file_is_pdf(gpointer ptr) {
  auto file_info = reinterpret_cast<NautilusFileInfo *>(ptr);
  auto maybe_pdf = false;

  if (!file_info) {
    return false;
  }
  auto parent = nautilus_file_info_get_parent_info(file_info);
  if (!parent) {
    return false;
  }
  if (!nautilus_file_info_is_directory(parent)) {
    g_object_unref(parent);
    return false;
  }
  if (!nautilus_file_info_can_write(parent)) {
    g_object_unref(parent);
    return false;
  }
  g_object_unref(parent);

  auto mime_type = nautilus_file_info_get_mime_type(file_info);
  if (strncmp(mime_type, "application/pdf", 16) == 0)
    maybe_pdf = true;
  else if (strncmp(mime_type, "application/postscript", 23) == 0)
    maybe_pdf = true;
  g_free(mime_type);

  return maybe_pdf;
}

static std::string new_path(GFile *orig_file) {
  GFile *parent_file, *new_file;
  char *basename, *extension, *new_basename;

  g_return_val_if_fail(G_IS_FILE(orig_file), NULL);

  parent_file = g_file_get_parent(orig_file);

  basename = g_strdup(g_file_get_basename(orig_file));

  extension = g_strdup(strrchr(basename, '.'));
  if (extension != NULL)
    basename[strlen(basename) - strlen(extension)] = '\0';

  new_basename = g_strdup_printf("%s%s%s", basename, ".ebook",
                                 extension == NULL ? "" : extension);
  g_free(basename);
  g_free(extension);

  new_file = g_file_get_child(parent_file, new_basename);

  g_object_unref(parent_file);
  g_free(new_basename);

  auto ptr = g_file_get_path(new_file);
  std::string rc(ptr);
  g_free(ptr);
  g_object_unref(new_file);
  return rc;
}

static void resize(NautilusFileInfo *file_info) {
  auto file = nautilus_file_info_get_location(file_info);
  auto path = g_file_get_path(file);

  if (path) {
    std::stringstream command;
    auto out = new_path(file);
    /* TODO: Options are:
     * /screen
     * /ebook
     * /prepress
     * /printer
     * /default
     */

    command
      << "gs -sDEVICE=pdfwrite -dCompatibilityLevel=1.4 -dEmbedAllFonts=true "
         "-dSubsetFonts=true -dPDFA=2 -dPDFACompatibilityPolicy=1 "
         "-dPDFSETTINGS=/ebook -dNOPAUSE -dQUIET -dBATCH -sOutputFile=\""
      << out << "\" \"" << path << "\"";
    std::system(command.str().c_str());
  }
  g_free(path);
}

static void pdf_resize_callback(NautilusMenuItem *item, GList *files) {
  for (auto file = files; file != nullptr; file = file->next) {
    auto f = reinterpret_cast<NautilusFileInfo *>(file->data);
    if (pdf_converter_file_is_pdf(f)) {
      resize(f);
    }
  }
}

static GList *
nautilus_pdf_converter_get_file_items(NautilusMenuProvider *provider,
                                      GtkWidget *window, GList *files) {
  NautilusMenuItem *item;
  GList *file;
  GList *items = nullptr;
  for (file = files; file != nullptr; file = file->next) {
    if (pdf_converter_file_is_pdf(file->data)) {
      item = nautilus_menu_item_new(
        "NautilusPdfConverter::resize", ("Resize PS/EPS/PDF..."),
        ("Resize each selected PS/EPS/PDF"), "stock_position-size");
      g_signal_connect(item, "activate", G_CALLBACK(pdf_resize_callback),
                       nautilus_file_info_list_copy(files));

      items = g_list_prepend(items, item);

      items = g_list_reverse(items);

      return items;
    }
  }

  return nullptr;
}

static GList *
nautilus_pdf_converter_get_background_items(NautilusMenuProvider *provider,
                                            GtkWidget *window,
                                            NautilusFileInfo *current_folder) {
  return nullptr;
}

static void
nautilus_pdf_converter_menu_provider_iface_init(gpointer g_iface,
                                                gpointer iface_data) {
  auto iface = reinterpret_cast<NautilusMenuProviderIface *>(g_iface);
  (void)iface_data;

  iface->get_background_items = nautilus_pdf_converter_get_background_items;
  iface->get_file_items = nautilus_pdf_converter_get_file_items;
}

extern "C" void nautilus_module_initialize(GTypeModule *module) {
  static const GTypeInfo info = {
    sizeof(NautilusPdfConverterClass),
    nullptr,
    nullptr,
    nautilus_pdf_converter_class_init,
    nullptr,
    nullptr,
    sizeof(NautilusPdfConverter),
    0,
    nautilus_pdf_converter_instance_init,
  };

  static const GInterfaceInfo menu_provider_iface_info = {
    nautilus_pdf_converter_menu_provider_iface_init, nullptr, nullptr};

  pdf_converter_type =
    g_type_module_register_type(module, G_TYPE_OBJECT, "NautilusPdfConverter",
                                &info, static_cast<GTypeFlags>(0));

  g_type_module_add_interface(module, pdf_converter_type,
                              NAUTILUS_TYPE_MENU_PROVIDER,
                              &menu_provider_iface_info);
}

extern "C" void nautilus_module_shutdown(void) {
}

extern "C" void nautilus_module_list_types(const GType **types,
                                           int *num_types) {
  static GType type_list[1];

  type_list[0] = pdf_converter_type;
  *types = type_list;
  *num_types = 1;
}
