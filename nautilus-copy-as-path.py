"""
Add an action to the Nautilus context-menu to copy full file path 

Author: Armin Novak
License: GPL-3
"""

import pyperclip
from gi.repository import GObject, Nautilus

class CopyAsFullPathMenuProvider(GObject.GObject, Nautilus.MenuProvider):
    def convert(self, menu, files):
        file_list = []
        for file in files:
            file_path = file.get_location().get_path()
            file_list.append(file_path)
        pyperclip.copy('\n'.join(file_list))


    def get_file_items(self, window, files):
        menu_item = Nautilus.MenuItem(
                        name="copy as path",
                        label="Copy path")

        menu_item.connect('activate', self.convert, files)

        return menu_item,
