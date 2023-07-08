from cs50 import get_string

def main():
    text = get_string("Text: ")
    letters = 0
    sentences = 0
    words = 1
    for c in text:
        if c.isalpha():
            letters += 1
        elif c.isspace():
            words += 1
        elif c == "!" or c == "?" or c == ".":
            sentences += 1
    index(letters, words, sentences)

def index(letters, words, sentences):
    L = (100/words) * letters
    S = (100/words) * sentences
    index = round(0.0588 * L - 0.296 * S - 15.8)
    if index < 1:
        print("Before Grade 1\n")
    elif index >= 16:
        print("Grade 16+\n")
    else:
        print(f"Grade {index}\n")



if __name__ == "__main__":
    main()