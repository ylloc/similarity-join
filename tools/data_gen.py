import random
import string

with open("tests/fixtures/strings.txt", "w") as f:
    for _ in range(1000):
        length = random.randint(18, 22)
        random_str = ''.join(random.choices(string.ascii_lowercase, k=length))
        f.write(random_str + '\n')
