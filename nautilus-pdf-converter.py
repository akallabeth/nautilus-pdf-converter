"""
Add an action to the Nautilus context-menu to resize pdf/ps/eps/...
to the ebook compatible pdf format

Author: Armin Novak
"""

import os
from urllib.parse import urlparse, unquote
from gi.repository import GObject, Nautilus

class ResizeForEbookPDFMenuProvider(GObject.GObject, Nautilus.MenuProvider):
    VALID_MIMETYPES = ('application/pdf', 'application/postscript')

    def convert(self, menu, files):
        for file in files:
            file_path = unquote(urlparse(file.get_uri()).path)
            file_split = os.path.splitext(file_path)
            file_base = file_split[0]

            file_new = file_base + '.ebook.pdf'
            count = 1
            while os.path.exists(file_new):
                file_new = file_base + '(' + str(count) + ').ebook.pdf'
                count+=1

            command  = 'gs -sDEVICE=pdfwrite -dCompatibilityLevel=1.4 -dEmbedAllFonts=true '
            command += '-dSubsetFonts=true -dPDFA=2 -dPDFACompatibilityPolicy=1 '
            command += '-dPDFSETTINGS=/ebook -dNOPAUSE -dQUIET -dBATCH -sOutputFile=\"'
            command += str(file_new) + '\" \"' + str(file_path) + '\"'
            os.system(command)

    def get_file_items(self, window, files):
        for file in files:
            if file.get_mime_type() not in self.VALID_MIMETYPES:
                return ()

        menu_item = Nautilus.MenuItem(
                        name="nautilus_resize_for_ebook_pdf",
                        label="Resize PS/EPS/PDF...")

        menu_item.connect('activate', self.convert, files)

        return menu_item,
