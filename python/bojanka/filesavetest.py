from tkinter import *
from tkinter.filedialog import askopenfilename, asksaveasfilename

window = Tk()
window.attributes("-topmost", True)
window.withdraw()

myFormats = [('Windows Text File','*.txt')]
file = asksaveasfilename(title="Open File")

print(file)