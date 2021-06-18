from characters import cyrillic, latin, two_let_chars

class Translator:
    def __init__(self):
        self.text = ""
        self.final_text = ""
        self.previous_char = ""
        self.is_latin = False
        self.is_tlc = False
        self.tlc_num = 1

    def check_tlc(self, char, index):
        for tlc in two_let_chars:
            if tlc[0] == self.previous_char and tlc[1] == char:
                self.final_text = list(self.final_text)
                self.final_text[index-self.tlc_num] = cyrillic[latin.index(tlc)]

                stringed_text = ""
                for let in self.final_text:
                    stringed_text += let
                self.final_text = stringed_text

                self.tlc_num += 1
                self.is_latin = True
                self.is_tlc = True

    def check_letter(self, char):
        if self.is_tlc: return
        for idx, letter in enumerate(latin):
            if char == letter:
                self.final_text += cyrillic[idx]
                self.is_latin = True

    def main(self):
        self.text = input("Унеси текст који хоћеш да преведеш на ћирилицу:")

        for index, char in enumerate(self.text):
            self.check_tlc(char, index)

            self.check_letter(char)

            if not self.is_latin:
                self.final_text += char

            self.previous_char = char
            self.is_latin = False
            self.is_tlc = False

        print(self.final_text)

if __name__ == "__main__":
    t = Translator()
    t.main()