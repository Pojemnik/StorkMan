import sys
import os
import re

keywords = { 'for', 'struct' }

def remove_comments(d):
    lines = d.splitlines()
    data = ''
    for l in lines:
        if l and l.strip()[0] != '/':
            data += l+'\n'
    return data

def tokenize(data):
    data = re.split(r'[ \t\n\r]', data)
    tokens = []
    for x in data:
        if x:
            tab = x.split('"')
            for t in tab[:-1]:
                tokens.append(t)
                tokens.append('"')
            if tab[-1]:
                tokens.append(tab[-1])
    return tokens

dirname = os.path.dirname(__file__)
path = os.path.join(dirname, 'in.xml')
if len(sys.argv) > 1:
    path = sys.argv[1]
data = None
with open(path, 'r', encoding='utf8') as f:
    data = f.read()
data = remove_comments(data)
tokens = tokenize(data)
in_string = False
for t in tokens:
    if t == '"':
        in_string = not in_string
        continue
    if t in keywords:
        print('aaa')

x = input()
