letters = "abcdefghijklmnopqrstuvwxyz"
digits = "0123456789"

#a Token stores what token and its attribute (ex: (FLOAT, 3.14))
class Token:
    def __init__(self, token, att):
        self.token = token
        self.att = att

    def get_precedence(self):
        if self.token == ',':
            return 0
        elif self.att == '=':
            return 1
        elif self.token == '||':
            return 2
        elif self.token == '&&':
            return 3
        elif self.token == '|':
            return 4
        elif self.token == '^':
            return 5
        elif self.token == '&':
            return 6
        elif self.token == '==' or self.token == '!=':
            return 7
        elif self.token == '<' or self.token == '>' or self.token == '<=' or self.token == '>=':
            return 8
        elif self.token == '<<' or self.token == '>>':
            return 9
        elif self.att == '+' or self.att == '-': #binary +/- have lower precedence than unary +/-
            return 10
        elif self.att == '*' or self.att == '/' or self.att == '%':
            return 11
        elif self.att == ')' or self.att == 'sentinel':
            return 13 #This could also be like -1 or something. More to come...
        else:
            print("Error: {} has no precedence".format(self))
        '''
#will be for unary ops &, *, +, -
        elif self.token == '':
            return 12
#will be for array access [], parens (), dot operator ., arrow operator ->, and left ro right inc and dec ++ --
        elif self.token == '':
            return 13
        '''
        
            
    def __str__(self):
        return "(" + self.token + ", " + self.att + ")"

#Lexer retrieves the next token from the source
class Lexer:
    #the file path to the code is passed in
    def __init__(self, code):
        self.code = code
        self.length = len(self.code)
        self.pos = 0

    def scan(self):
        while self.code[self.pos] in " \n\t":
            self.pos += 1

        lexeme_begin = self.pos
        forward = self.pos
        
        #scanning for identifiers
        if self.code[forward] in letters + '_':
            forward += 1
            while self.code[forward] in letters + digits + '_':
                forward += 1
            self.pos = forward
            return Token("ID", self.code[lexeme_begin:forward])          #XXX: Make sure to add in an array of keywords, and if the id matches a keyword new token

        #scanning for integers and floating point numbers
        forward = self.pos
        if self.code[forward] in digits:
            while self.code[forward] in digits:
                forward += 1

            if self.code[forward] == '.':
                forward += 1
                if self.code[forward] in digits:
                    while self.code[forward] in digits:
                        forward += 1
                    else:
                        if self.code[forward] not in letters + '_':
                            self.pos = forward
                            return Token("FLOAT", self.code[lexeme_begin:forward])

            if self.code[forward] not in letters + '_':
                self.pos = forward
                return Token("INT", self.code[lexeme_begin:forward])

        #some math ops
        forward = self.pos
        if self.code[forward] == '+':
            self.pos += 1
            return Token("add_op", '+')
        
        if self.code[forward] == '-':
            self.pos += 1
            return Token("add_op", '-')
        
        if self.code[forward] == '*':
            self.pos += 1
            return Token("mult_op", '*')
        
        if self.code[forward] == '/':
            self.pos += 1
            return Token("mult_op", '/')

        #parens
        if self.code[forward] == '(':
            self.pos += 1
            return Token("paren", '(')

        if self.code[forward] == ')':
            self.pos += 1
            return Token("paren", ')')

        if self.code[forward] == ';':
            self.pos += 1
            return Token("semicolon", ';')

        #checking for end of file token
        forward = self.pos
        if self.code[forward] in '$':
            forward += 1
            if self.code[forward] in '$':
                forward += 1
                self.pos = forward
                return Token("EOF", "$$")

        #when the string isnt accepted by any of the state machines
        return Token("Error", "Error")
