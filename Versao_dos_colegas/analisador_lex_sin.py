TKFim = -1
TKErro = -2
TKIndef = -3

TKId = 1
TKWhile = 2
TKFor = 3
TKSwitch = 4
TKCase = 5
TKOtherwise = 6
TKEnd = 7
TKIf = 8
TKElse = 9
TKElseIf = 10
TKAtrib = 11
TKSoma = 12
TKAbrePar = 13
TKFechaPar = 14
TKAbreChave = 15
TKFechaChave = 16
TKVirgula = 17
TKPontoeVirg = 18
TKString = 19
TKAbreColchete = 20
TKFechaColchete = 21
TKSubtracao = 22
TKMultiplicacao = 23
TKPotencia = 24
TKDivisao = 25
TKComentario = 26
TKConstanteInteira = 27
TKIgual = 28
TKDiferente = 29
TKMaior = 30
TKMaiorIgual = 31
TKMenor = 32
TKMenorIgual = 33
TKEBinario = 34
TKELogico = 35
TKOuLogico = 36
TKOuBinario = 37
TKConstanteReal = 38
TKFunction = 39
TKBreak = 40
TKParfor = 41
TKTry = 42
TKCatch = 43
TKContinue = 44
TKReturn = 45
TKTrue = 46
TKFalse = 47

pos = 0
linha = 1
coluna = 0

class pal_res():
    palavra = ""
    tk = 0

    def __init__(self, palavra, tk):
        self.palavra = palavra
        self.tk = tk

lista_pal = [
	pal_res("while", TKWhile),
	pal_res("for", TKFor),
	pal_res("switch", TKSwitch),
	pal_res("case", TKCase),
	pal_res("otherwise", TKOtherwise),
	pal_res("end", TKEnd),
	pal_res("if", TKIf),
	pal_res("else", TKElse),
	pal_res("elseif", TKElseIf),
	pal_res("function", TKFunction),
	pal_res("break", TKBreak),
	pal_res("parfor", TKParfor),
	pal_res("try", TKTry),
	pal_res("catch", TKCatch),
	pal_res("continue", TKContinue),
	pal_res("return", TKReturn),
	pal_res("true", TKTrue),
   	pal_res("false", TKFalse)
]

def palavra_reservada(lex):
    for pal in lista_pal:
        if lex == pal.palavra:
            return pal.tk
    return TKId

def reset_col():
    coluna = 0
    linha += 1

def isNumber(num):
    if num >= '0' and num <= '9':
        return True
    else:  
        return False

def isLetter(letter):
    if letter >= 'a' and letter <= 'z' or letter >= 'A' and letter <= 'Z':
        return True
    else:
        return False

def rec_equ(st):
    estado = 0
    fim = False
    posl = 0
    palavra = ""
    c = ''
    lex = ''

    def procuraTK():
        if estado == 0:
            coluna += 1
            pos += 1

            if c == ' ' or c == '\t':
                posl -= 1
            elif c == '\n':
                posl -= 1
                reset_col()
            elif isLetter(c):
                estado = 1
            elif isNumber(c):
                estado = 7
            elif c == '.':
                estado = 14
            elif c == '%':
                estado = 3
            elif c == '=':
                estado = 8
            elif c == '~':
                estado = 9
            elif c == '>':
                estado = 10
            elif c == '<':
                estado = 11
            elif c == '&':
                estado = 12
            elif c == '|':
                estado = 13
            elif c == '+':
                return TKSoma
            elif c == '-':
                return TKSubtracao
            elif c == '*':
                return TKMultiplicacao
            elif c == '/':
                return TKDivisao
            elif c == '(':
                return TKAbrePar
            elif c == ')':
                return TKFechaPar
            elif c == '{':
                return TKAbreChave
            elif c == '}':
                return TKFechaChave
            elif c == '[':
                return TKAbreColchete
            elif c == ']':
                return TKFechaColchete
            elif c == ',':
                return TKVirgula
            elif c == ';':
                return TKPontoeVirg
            elif c == '_':
                return TKErro
            else:
                return TKFim

        elif estado == 1:
            if isLetter(c) or isNumber(c) or c == '_':
                pos += 1
                return
            posl -= 1
            lex[posl] = ''
            palavra = palavra_reservada(lex)
            return palavra

        elif estado == 2:
            pos += 1
			if c == '\n':
				return TKErro
            elif c != '\'':
                return
            return TKString

        elif estado == 3:
            if c == '{':
                estado = 4
            elif c == '\n':
                posl -= 1
                lex[posl] = ''
                return TKComentario
            else:
                estado = 6
            pos += 1

        elif estado == 4:
            if c == '%':
                estado = 5
            pos += 1

        elif estado == 5:
            pos += 1
			if c == '%':
				break
            elif c != '}':
                estado = 4
                return
            return TKComentario

        elif estado == 6:
            if c != '\n':
                pos += 1
                return
            posl -= 1
            lex[posl] = ''
            return TKComentario

        elif estado == 7:
            pos += 1
            if isNumber(c):
                return
            elif c == '.':
                estado = 14
                return
            elif c == 'e':
                estado = 15
                return
            posl -= 1
            lex[posl] = ''
            return TKConstanteInteira

        elif estado == 14:
            pos += 1
            if isNumber(c):
                return
            elif c == 'e':
                estado = 15
                return
            elif c == '.':
                return TKErro
            posl -= 1
            lex[posl] = ''
            return TKConstanteReal

        elif estado == 15:
            pos += 1
            if isNumber(c):
                return
            elif c == '.' or c == 'e':
                return TKErro
            posl -= 1
            lex[posl] = ''
            return TKConstanteReal

        elif estado == 8:
            if c == '=':
                pos += 1
                return TKIgual
            else:
                posl -= 1
                lex[posl] = ''
                return TKAtrib

        elif estado == 9:
            if c == '=':
                pos += 1
                return TKDiferente

        elif estado == 10:
            if c == '=':
                pos += 1
                return TKMaiorIgual
            else:
                posl -= 1
                lex[posl] = ''
                return TKMaior

        elif estado == 11:
            if c == '=':
                pos += 1
                return TKMenorIgual
            else:
                posl -= 1
                lex[posl] = ''
                return TKMenor

        elif estado == 12:
            if c == '&':
                pos += 1
                return TKELogico
            else:
                posl -= 1
                lex[posl] = ''
                return TKEBinario

        elif estado == 13:
            if c == '|':
                pos += 1
                return TKOuLogico
            else:
                posl -= 1
                lex[posl] = ''
                return TKOuBinario

    while not fim:
        c = st[pos]
        lex[posl] = c
        posl += 1
        tk = procuraTK()
        if tk:
            return tk, lex

#main
with open("Entrada.m", 'r') as fp:
    characters = fp.read()
    fp.close()

with open("Saida.lex", "w") as nf:
    linha = 1
    coluna = 0
    tk = True

    while tk:
        tk, lex = rec_equ(characters)
        if tk != TKFim:
            if tk == TKErro:
                print("Ocorreu um erro lexico")
                break
            print("Token: %d\t Linha: %d\t Coluna: %d\tLex: %s \n", tk, linha, coluna, lex);
            nf.write("Token: %d\t Linha: %d\t Coluna: %d\tLex: %s \n", tk, linha, coluna, lex);
            coluna += len(lex) - 1
            if lex[len(lex - 1)] == '\n':
                linha += 1

    nf.close()
	print("Arquivo gerado com sucesso")
