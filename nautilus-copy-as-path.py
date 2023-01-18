"""
Add an action to the Nautilus context-menu to copy full file path 

Author: Armin Novak
License: GPL-3
"""

import pyperclip
from urllib.parse import urlparse, unquote
from gi.repository import GObject, Nautilus

class CopyFullPathMenuProvider(GObject.GObject, Nautilus.MenuProvider):
    def convert(self, menu, files):
        file_list = []
        for file in files:
            file_path = unquote(urlparse(file.get_uri()).path)
            file_list.append(file_path)
        pyperclip.copy('\n'.join(file_list))


    def get_file_items(self, window, files):
        menu_item = Nautilus.MenuItem(
                        name="copy as path",
                        label="Copy path")

        menu_item.connect('activate', self.convert, files)

        return menu_item,
