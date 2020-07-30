import sys
import Lexer as Lex

class Stack:
    def __init__(self):
        self.elements = []

    def push(self, element):
        self.elements.append(element)

    def pop(self):
        element = self.elements[len(self.elements)-1]
        del self.elements[len(self.elements)-1]
        return element

    def peek(self):
        if self.elements != []:
            return self.elements[len(self.elements)-1]

    def is_empty(self):
        return self.elements == []

    def __str__(self):
        return str([str(i) for i in self.elements])


class Parser:                                                     #can manually set the lexer's self.pos variable like passing in &pos in C
    def __init__(self, path):
        self.path = path
        self.file = open(path, 'r')
        self.code = self.file.read() + '$$'
        self.file.close()
        self.lexer = Lex.Lexer(self.code)
        self.operands = Stack()
        self.operators = Stack()
        self.operators.push(Lex.Token('sentinel', 'sentinel'))

    def parse(self):
        if self.parse_expr():
            tok = self.lexer.scan()
            if tok.token == "semicolon":
                print("Successfully parsed: {}".format(self.lexer.code[:self.lexer.pos]))
                self.shunting_yard(0, self.lexer.pos)
                return
        print("Failure")
        print(self.operands, self.operators)

    #expr -> factor {binary_op factor}
    def parse_expr(self):
        if not self.parse_factor():
            return 0

        while True:
            if not self.parse_binary_op():
                return 1

            if not self.parse_factor():
                return 0
        
    
    #factor -> int | (expr) | id | unary_op factor
    def parse_factor(self):
        pos_0 = self.lexer.pos
        tok = self.lexer.scan()

        if tok.token == "INT":
            self.operands.push(int(tok.att))
            return 1

        if tok.token == "ID":
            return 1

        if tok.att == "(":             #XXX supposed to push the sentinel value onto the stack of operators here
            if self.parse_expr():
                tok = self.lexer.scan()
                if tok.att == ")":
                    return 1

        #reset position in text file on failure
        self.lexer.pos = pos_0
        return 0

    #binary_op -> mult_op | add_op
    def parse_binary_op(self):
        pos_0 = self.lexer.pos
        tok = self.lexer.scan()

        if tok.token == "add_op":
            top = self.operators.peek()

            #wait for next operand to know if you should do something yet
            if top.token == 'sentinel':
                self.operators.push(tok)
                return 1

            while top.token == 'mult_op':
                self.operators.pop()
                v2 = self.operands.pop()
                v1 = self.operands.pop()
                if top.att == '*':
                    self.operands.push(v1 * v2)
                elif top.att == '/':
                    self.operands.push(v1 / v2)
                else:
                    print("Error in parse_binary_op")
                    sys.exit(0)
                top = self.operators.peek()

            if top.token == 'add_op':
                self.operators.pop()
                v2 = self.operands.pop()
                v1 = self.operands.pop()
                if top.att == '+':
                    self.operands.push(v1 + v2)
                elif top.att == '-':
                    self.operands.push(v1 - v2)
                else:
                    print("Error in parse_binary_op")
                    sys.exit(0)
                top = self.operators.peek()
            self.operators.push(tok)
            return 1

        if tok.att == 'mult_op':
            top = self.operators.peek()
            if top.att == 'sentinel':
                self.operators.push(tok)
                return 1

            while top.token == 'mult_op':
                self.operators.pop()
                v2 = self.operands.pop()
                v1 = self.operands.pop()
                if top.att == '*':
                    self.operands.push(v1 * v2)
                elif top.att == '/':
                    self.operands.push(v1 / v2)
                else:
                    print("Error in parse_binary_op")
                    sys.exit(0)
                top = self.operators.peek()

            self.operators.push(tok)
            return 1

        #reset position in text file on failure
        self.lexer.pos = pos_0
        return 0

    #unary_op -> + | -          Eventually *, &, etc
    def parse_unary_op(self):
        pass
"""
    def shunting_yard(self, start, end):
        sy_lexer = Lex.Lexer(self.code[start:end])

        tok = sy_lexer.scan()
        while tok.att != ';':
            print("current token: {}".format(tok))
            
            #push all numbers to operands stack
            if tok.token == 'INT':
                self.operands.push(int(tok.att))

            #open parens jsut get pushed to mark for close parens
            elif tok.att == '(':
                self.operators.push(Lex.Token('sentinel', 'sentinel'))

            '''
            #evaluate the inside of parentheses
            elif tok.att == ')':
                #get rid of the open paren
                self.operators.pop()

                #get the last operator seen from the stack
                top = self.operators.peek()

                #while we haven't seen  XXX: used to have "top.att != '(' and" at the front of if condition
                while tok.get_precedence() <= top.get_precedence():
                    top = self.operators.pop()
                    if top.att == 'sentinel':
                        break
                    print("top is: {}".format(top))
                    v2 = self.operands.pop()
                    v1 = self.operators.pop()
                    if top.att == '*':
                        self.operands.push(v1 * v2)
                    elif top.att == '/':
                        self.operands.push(v1 / v2)
                    elif top.att == '+':
                        self.operands.push(v1 + v2)
                    elif top.att == '-':
                        self.operands.push(v1 - v2)
                    else:
                        print("Error in shunting yard paren while loop")
                        sys.exit(0)
                self.operators.pop()'''
            elif tok.att == '*' or tok.att == '/' or tok.att == '+' or tok.att == '-':
                #XXX
                if self.operators.is_empty():
                    self.operators.push(tok)
                    tok = sy_lexer.scan()
                    continue

                top = self.operators.peek()
                #usual condition is that while operators isnt empty
                while not top.att == 'sentinel' and tok.get_precedence() <= top.get_precedence():
                    print("***tok: {}, top: {}, operators: {}, operands: {}".format(tok, top, self.operators, self.operands))
                    #if we haven't yet seen the other operand we need to hop out
                    if len(self.operands.elements) == 1:
                        break
                    
                    top = self.operators.pop()
                    if tok.get_precedence() > top.get_precedence():
                        break
                    
                    v2 = self.operands.pop()
                    v1 = self.operands.pop()
                    if top.att == '*':
                        self.operands.push(v1 * v2)
                    elif top.att == '/':
                        self.operands.push(v1 / v2)
                    elif top.att == '+':
                        self.operands.push(v1 + v2)
                    elif top.att == '-':
                        self.operands.push(v1 - v2)
                    else:
                        print("Error in shunting yard regular while loop")
                        sys.exit(0)

                    print("immediately after1: opoerators: {}, operands: {}".format(self.operators, self.operands))
                print("immediately after2: opoerators: {}, operands: {}".format(self.operators, self.operands))
                self.operators.push(tok)
            else:
                print("God only knows (in shunting yard)", tok)

            tok = sy_lexer.scan()
"""      
                    

"""
lexer = Lexer("test_source.txt")

while True:
    tok = lexer.scan()
    print(tok)
    if tok.token == "EOF":
        break
"""

parser = Parser("test_source.txt")
parser.parse()





