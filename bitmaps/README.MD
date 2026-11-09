# Bitmaps

To add more bitmaps:
- Create a new folder (Named descriptively)
- Upload bitmap (.bmp) versions for future editing and use
- Convert bitmap to hex binary (.BIN) and upload
    - Any bitmaps for use with plot_20l_bitmap() (ex. bgs) must be upside down with both halves reversed since the function is optimized with movem.l
    ex.
    |1234|  --> |6587|
    |5678|      |2143|
- Add a new entry to bitmaps.s using the existing format
    - If bitmaps.s is more than 65535 bytes, create a new file (bitmap1.s) and add an entry there
    - If a new file was needed, add it to the makefile in the root folder
- Add a new entry to bitmaps.h
