class level:
    def __init__(self, name):
        self.content = []
        self.content.append('<level name="{0}" background="bg1">'.format(name))

    
    def save(self, path):
        self.content.append('</level>')
        with open(path, 'w') as out_file:
            out_file.write('\n'.join(self.content))
        
    def add(self, s):
        self.content.append(s)