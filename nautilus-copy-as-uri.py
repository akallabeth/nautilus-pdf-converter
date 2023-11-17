"""
Add an action to the Nautilus context-menu to copy uri to clipboard

Author: Armin Novak
License: GPL-3
"""

import os
import gettext
import pyperclip
from gi.repository import GObject, Nautilus

class CopyAsUriMenuProvider(GObject.GObject, Nautilus.MenuProvider):
    def translate(self):
        f = os.path.basename(__file__)
        name, ext = os.path.splitext(f)
        gettext.install(name)

    def convert(self, menu, files):
        file_list = []
        for file in files:
            file_path = file.get_uri()
            file_list.append(file_path)
        pyperclip.copy('\n'.join(file_list))


    def get_file_items(self, *args):
        files = args[-1]
        if len(files) < 1:
            return
        self.translate()
        menu_item = Nautilus.MenuItem(
                        name="copy-as-uri",
                        label=_("Copy as URI"))

        menu_item.connect('activate', self.convert, files)

        return menu_item,
