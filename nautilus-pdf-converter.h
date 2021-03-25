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
#ifndef NAUTILUSPDFCONVERTER_H
#define NAUTILUSPDFCONVERTER_H

#include <glib-object.h>

G_BEGIN_DECLS

#define NAUTILUS_TYPE_PDF_CONVERTER (nautilus_pdf_converter_get_type())
#define NAUTILUS_PDF_CONVERTER(o)                                              \
  (G_TYPE_CHECK_INSTANCE_CAST((o), NAUTILUS_TYPE_PDF_CONVERTER,                \
                              NautilusPdfConverter))
#define NAUTILUS_IS_PDF_CONVERTER(o)                                           \
  (G_TYPE_CHECK_INSTANCE_TYPE((o), NAUTILUS_TYPE_PDF_CONVERTER))
typedef struct _NautilusPdfConverter NautilusPdfConverter;
typedef struct _NautilusPdfConverterClass NautilusPdfConverterClass;

struct _NautilusPdfConverter {
  GObject parent_slot;
};

struct _NautilusPdfConverterClass {
  GObjectClass parent_slot;
};

G_END_DECLS

#endif // NAUTILUSPDFCONVERTER_H
