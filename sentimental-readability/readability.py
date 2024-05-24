# TODO
import cs50
import math

def main():
    text = cs50.get_string("Text: ")
    letters = int(count_letters(text))
    words = int(count_words(text))
    sentences = int(count_sentences(text))

    avgLetters = float((float(letters) / float(words)) * 100)
    avgSentences = float((float(sentences) / float(words)) * 100)
    index = int(round(0.0588 * avgLetters - 0.296 * avgSentences - 15.8))

    if index > 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


def count_sentences(text):
    # ASCII !,.,? -> 33, 46, 63
    # All words except the first are preceded by a space. As such, 1 is added to the return value.
    sentences = 0

    for ch in text:
        if ch in ["!", "?", "."]:
            sentences += 1

    return sentences


def count_words(text):
    # ASCII Space -> 32
    # All words except the first are preceded by a space. As such, 1 is added to the return value.
    words = 0

    for ch in text:
        if ch == " ":
            words += 1

    return words + 1


def count_letters(text):
    # ASCII A-Z -> 65 to 90
    # ASCII a-z -> 97 to 122
    letters = 0

    for ch in text:
        ch = str(ch)
        if ch.isalpha():
            letters += 1

    return letters

main()