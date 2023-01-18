"""
Add an action to the Nautilus context-menu to copy uri to clipboard 

Author: Armin Novak
License: GPL-3
"""

import pyperclip
from gi.repository import GObject, Nautilus

class CopyAsUriMenuProvider(GObject.GObject, Nautilus.MenuProvider):
    def convert(self, menu, files):
        file_list = []
        for file in files:
            file_path = file.get_uri()
            file_list.append(file_path)
        pyperclip.copy('\n'.join(file_list))


    def get_file_items(self, window, files):
        menu_item = Nautilus.MenuItem(
                        name="Copy as URI",
                        label="Copy URI")

        menu_item.connect('activate', self.convert, files)

        return menu_item,
