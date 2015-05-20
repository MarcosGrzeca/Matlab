#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TKFim -1
#define TKErro -2
#define TKId 1
#define TKWhile 2
#define TKFor 3
#define TKSwitch 4
#define TKCase 5
#define TKOtherwise 6
#define TKEnd 7
#define TKIf 8
#define TKElse 9
#define TKElseIf 10
#define TKAtrib 11
#define TKSoma 12
#define TKAbrePar 13
#define TKFechaPar 14
#define TKAbreChave 15
#define TKFechaChave 16
#define TKVirgula 17
#define TKPontoeVirg 18
#define TKString 19
#define TKAbreColchete 20
#define TKFechaColchete 21
#define TKSubtracao 22
#define TKMultiplicacao 23
#define TKPotencia 24
#define TKDivisao 25
#define TKComentario 26
#define TKConstanteInteira 27
#define TKIgual 28
#define TKDiferente 29
#define TKMaior 30
#define TKMaiorIgual 31
#define TKMenor 32
#define TKMenorIgual 33
#define TKEBinario 34
#define TKELogico 35
#define TKOuLogico 36
#define TKOuBinario 37
#define TKConstanteReal 38
#define TKFunction 39
#define TKBreak 40
#define TKParfor 41
#define TKTry 42
#define TKCatch 43
#define TKContinue 44
#define TKReturn 45
#define TKTrue 46
#define TKFalse 47
#define TKDoisPontos 48
#define TKNegacao 49
#define BUFSIZEINI 1000

struct pal_res{
	char palavra[20];
	int tk;
};

struct pal_res lista_pal[] =
{
	{ "while", TKWhile },
	{ "for", TKFor },
	{ "switch", TKSwitch },
	{ "case", TKCase },
	{ "otherwise", TKOtherwise },
	{ "end", TKEnd },
	{ "if", TKIf },
	{ "else", TKElse },
	{ "elseif", TKElseIf },
	{ "function", TKFunction },
	{ "break", TKBreak },
	{ "parfor", TKParfor },
	{ "try", TKTry },
	{ "catch", TKCatch },
	{ "continue", TKContinue },
	{ "return", TKReturn },
	{ "true", TKTrue },
	{ "false", TKFalse },
	{ "fimtabela", TKId }
};

struct token {
	int tk;
	int linha;
	int coluna;
	char elemento[200];
};

struct _ret{
	int ret;
	//char cod[1000];
	char *cod;
	void *place;
};

int tk;
int linha = 1, coluna = 0;
char* characters;
struct token* tokens;
int pos = 0, posTK = -1, ultPosTK = -1;
FILE * newFile;
FILE * portugues;
size_t space = 1;
struct _ret* s_ret;

char variaveis[100][30];
int nroVariaveis = 0;

int buscaTS(char variavel[]) {
	int indice;

	for (indice = 0; indice < nroVariaveis; indice++) {
		if (strcmp(variavel, variaveis[indice]) == 0) {
			return 1;
		}
	}
	return 0;
}

void incluirTS(char variavel[]) {

	if (buscaTS(variavel) == 0) {
		strncpy_s(variaveis[nroVariaveis], variavel,29);
		nroVariaveis++;
	}
}

void escreverStringPort(char string) {
	fprintf(portugues, "%s \n", string);
}

int palavra_reservada(char lex[])
{
	int postab = 0;
	while (strcmp("fimtabela", lista_pal[postab].palavra) != 0)
	{
		if (strcmp(lex, lista_pal[postab].palavra) == 0)
		{
			return lista_pal[postab].tk;
		}
		postab++;
	}
	return TKId;
}

int rec_equ(char st[], char lex[], int * linha, int * coluna)
{
	int estado = 0,
		fim = 0,
		posl = 0,
		palavra;

	while (!fim)
	{
		char c = st[pos];
		lex[posl++] = c;
		switch (estado)
		{
		case 0:
			(*coluna) += 1;
			if (c == '\0')
			{
				return TKFim;
			}
			pos++;
			if (c == ' ' || c == '\t')
			{
				posl--;
				break;
			}
			if (c == '\n')
			{
				posl--;
				(*linha) += 1;
				(*coluna) = 0;
				break;
			}
			if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
			{
				estado = 1;
				break;
			}
			if (c >= '0' && c <= '9') { estado = 7; break; }
			if (c == '.') { estado = 14; break; }
			if (c == '\'') { estado = 2; break; }
			if (c == '%') { estado = 3; break; }
			if (c == '=') { estado = 8; break; }
			if (c == '~') { estado = 9; break; }
			if (c == '>') { estado = 10; break; }
			if (c == '<') { estado = 11; break; }
			if (c == '&') { estado = 12; break; }
			if (c == '|') { estado = 13; break; }

			lex[posl] = '\0';
			if (c == '+') { return TKSoma; }
			if (c == '-') { return TKSubtracao; }
			if (c == '*') { return TKMultiplicacao; }
			if (c == '^') { return TKPotencia; }
			if (c == '/') { return TKDivisao; }
			if (c == '(') { return TKAbrePar; }
			if (c == ')') { return TKFechaPar; }
			if (c == '{') { return TKAbreChave; }
			if (c == '}') { return TKFechaChave; }
			if (c == '[') { return TKAbreColchete; }
			if (c == ']') { return TKFechaColchete; }
			if (c == ',') { return TKVirgula; }
			if (c == ';') { return TKPontoeVirg; }
			if (c == ':') { return TKDoisPontos; }
			return TKErro;
		case 1:
			if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9' || c == '_')
			{
				pos++;
				break;
			}
			lex[--posl] = '\0';
			palavra = palavra_reservada(lex);
			return palavra;
		case 2:
			pos++;
			if (c == '\n')
			{
				return TKErro;
			}
			if (c != '\'')
			{
				break;
			}
			lex[posl] = '\0';
			return TKString;
		case 3:
			if (c == '{')
			{
				estado = 4;
			}
			else if (c == '\n')
			{
				posl = 0;
				//return TKComentario;
				estado = 0;
				break;
			}
			else
			{
				estado = 6;
			}
			pos++;
			break;
		case 4:
			if (c == '%')
			{
				estado = 5;
			}
			if (c == '\n')
			{
				(*linha) += 1;
				(*coluna) = 0;
			}
			pos++;
			break;
		case 5:
			pos++;
			if (c == '%')
			{
				break;
			}
			else if (c != '}')
			{
				estado = 4;
				break;
			}
			posl = 0;
			//return TKComentario;
			estado = 0;
			break;
		case 6:
			if (c != '\n' && c != '\0')
			{
				pos++;
			}
			else
			{
				posl = 0;
				//return TKComentario;
				estado = 0;
			}
			break;
		case 7:
			if (c >= '0' && c <= '9')
			{
				pos++;
				break;
			}
			else if (c == '.')
			{
				pos++;
				estado = 14;
				break;
			}
			else if (c == 'e')
			{
				pos++;
				estado = 15;
				break;
			}
			lex[--posl] = '\0';
			return TKConstanteInteira;
		case 14:
			if (c >= '0' && c <= '9')
			{
				pos++;
				break;
			}
			if (c == 'e')
			{
				pos++;
				estado = 15;
				break;
			}
			if (c == '.')
			{
				pos++;
				return TKErro;
			}
			lex[--posl] = '\0';
			return TKConstanteReal;
		case 15:
			if (c >= '0' && c <= '9')
			{
				pos++;
				break;
			}
			if (c == '.' || c == 'e')
			{
				pos++;
				return TKErro;
			}
			lex[--posl] = '\0';
			return TKConstanteReal;
		case 8:
			if (c == '=')
			{
				pos++;
				lex[posl] = '\0';
				return TKIgual;
			}
			else
			{
				lex[--posl] = '\0';
				return TKAtrib;
			}
			break;
		case 9:
			if (c == '=')
			{
				pos++;
				lex[posl] = '\0';
				return TKDiferente;
			}
			else
			{
				return TKNegacao;
			}
			break;
		case 10:
			if (c == '=')
			{
				pos++;
				lex[posl] = '\0';
				return TKMaiorIgual;
			}
			else
			{
				lex[--posl] = '\0';
				return TKMaior;
			}
			break;
		case 11:
			if (c == '=')
			{
				pos++;
				lex[posl] = '\0';
				return TKMenorIgual;
			}
			else
			{
				lex[--posl] = '\0';
				return TKMenor;
			}
			break;
		case 12:
			if (c == '&')
			{
				pos++;
				lex[posl] = '\0';
				return TKELogico;
			}
			else
			{
				lex[--posl] = '\0';
				return TKEBinario;
			}
			break;
		case 13:
			if (c == '|')
			{
				pos++;
				lex[posl] = '\0';
				return TKOuLogico;
			}
			else
			{
				lex[--posl] = '\0';
				return TKOuBinario;
			}
		}
	}
}

void leToken()
{
	char lex[200];

	posTK++;
	if (posTK > ultPosTK)
	{
		tk = rec_equ(characters, lex, &linha, &coluna);
		if (tk == TKErro)
		{
			printf("Token desconhecido!\tLinha: %d\tColuna: %d\n", linha, coluna);
			return;
		}
		if (tk == TKFim)
		{
			return;
		}
		ultPosTK = posTK;
		tokens = (struct token *)realloc(tokens, sizeof(struct token) * space);
		tokens[posTK].tk = tk;
		tokens[posTK].linha = linha;
		tokens[posTK].coluna = coluna;
		//tokens[posTK].elemento = lex;
		strncpy_s(tokens[posTK].elemento,lex,sizeof(lex));
		fprintf(newFile, "Token: %d\t Linha: %d\t Coluna: %d\tLex: %s \n", tk, linha, coluna, lex);
		coluna += strlen(lex) - 1;
		if (lex[strlen(lex) - 1] == '\n')
		{
			linha++;
		}

	}
	else
	{
		tk = tokens[posTK].tk;
	}
}

void msgErro(char* msg)
{
	printf("Erro\n");
	printf("%s \tLinha: %d \tColuna: %d\n", msg, tokens[posTK].linha, tokens[posTK].coluna);
}

void erroAbrePar()
{
	msgErro("Parenteses esperado!");
}

void erroFechaPar()
{
	msgErro("Faltou fechar os parenteses!");
}

void erroExpInvalida()
{
	msgErro("Expressao invalida!");
}

void erroEnd()
{
	msgErro("END esperado!");
}

void erroVal()
{
	msgErro("Falta informar o valor!");
}

void erroDoisPt()
{
	msgErro("Dois pontos esperado!!");
}

void ocorreuErro()
{
	msgErro("Ocorreu um erro");
}

void voltaPos(int posicao)
{
	posTK = posicao;
	tk = tokens[posTK].tk;
}

int setPos()
{
	return posTK;
}

_ret *EXP0();
_ret *FUNCTION(int nivel);
_ret *EXP1();
_ret *COMP0();
_ret *BLOCO(int nivel);
_ret *VAL();

_ret *id(int salvarTabSimb)
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKId)
	{
		strncat_s(analise->cod,1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		if (salvarTabSimb == 1) {
			incluirTS(tokens[posTK].elemento);
		}
		leToken();
		analise->ret = 1;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *cte()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod,1000, "", strlen(""));
	if (tk == TKConstanteInteira)
	{
		strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		analise->ret = 1;
		return analise;
	}
	if (tk == TKConstanteReal)
	{
		strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		analise->ret = 1;
		return analise;
	}
	if (tk == TKString)
	{
		strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		analise->ret = 1;
		return analise;
	}
	if (tk == TKTrue)
	{
		strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		analise->ret = 1;
		return analise;
	}
	if (tk == TKFalse)
	{
		strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		analise->ret = 1;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *ATRIB(int nivel, int origem)
{

	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	_ret *ident = id(1);
	if (ident->ret)
	{
		if (origem == 0) {
			for (int i = 0; i < nivel; i++) {
				strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
			}
		}
		strncat_s(analise->cod, BUFSIZEINI, ident->cod, strlen(ident->cod));
		if (tk == TKAtrib)
		{
			//strncat_s(analise->cod,1000, tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			if (origem == 1) {
				strncat_s(analise->cod, BUFSIZEINI, " de ", strlen(" de "));
			} else {
				strncat_s(analise->cod, BUFSIZEINI, " <- ", strlen(" <- "));
			}
			leToken();
			_ret *val = VAL();
			if (val->ret)
			{
				analise->ret = 1;
				strncat_s(analise->cod, BUFSIZEINI, val->cod, strlen(val->cod));
				return analise;
			}
			erroVal();
			analise->ret = 0;
			return analise;
		}
		//REMOVER ULTIMA VARIAVEL
		if (nroVariaveis > 0) {
			nroVariaveis--;
		}
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *EXPFIM()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));

	if (tk == TKAbrePar)
	{
		strncpy_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *exp1 = EXP1();
		if (exp1->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, exp1->cod, strlen(exp1->cod));
			if (tk == TKFechaPar)
			{
				strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				leToken();
				analise->ret = 1;
				return analise;
			}
			erroFechaPar();
			analise->ret = 0;
			return analise;
		}
		analise->ret = 0;
		return analise;
	}
	int marcaPos = setPos();
	_ret *ident = id(1);
	if (ident->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, ident->cod, strlen(ident->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *cons = cte();
	if (cons->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, cons->cod, strlen(cons->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *function = FUNCTION(0);
	if (function->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, function->cod, strlen(function->cod));
		analise->ret = 1;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *EXP15()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKSubtracao)
	{
		strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *expFim = EXPFIM();
		if (expFim->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, expFim->cod, strlen(expFim->cod));
			analise->ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise->ret = 0;
		return analise;
	}
	else {
		_ret *expFim2 = EXPFIM();
		if (expFim2->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, expFim2->cod, strlen(expFim2->cod));
			analise->ret = 1;
			return analise;
		} else {
			analise->ret = 0;
			return analise;
		}
	}
	analise->ret = 0;
	return analise;
}

_ret *EXP14()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKPotencia)
	{
		strncat_s(analise->cod, BUFSIZEINI, " ", strlen(" "));
		strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		strncat_s(analise->cod, BUFSIZEINI, " ", strlen(" "));
		leToken();
		_ret *exp15 = EXP15();
		if (exp15->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, exp15->cod, strlen(exp15->cod));
			if (tk == TKPotencia)
			{
				strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				_ret *exp14 = EXP14();
				if (exp14->ret)
				{
					strncat_s(analise->cod, BUFSIZEINI, exp14->cod, strlen(exp14->cod));
					analise->ret = 1;
					return analise;
				}
				analise->ret = 0;
				return analise;
			}
			analise->ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *EXP13()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));

	_ret *exp15 = EXP15();
	if (exp15->ret)
	{
		strncpy_s(analise->cod, 1000, exp15->cod, strlen(exp15->cod));
		if (tk == TKPotencia)
		{
		//	strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			_ret *exp14 = EXP14();
			if (exp14->ret)
			{
				strncat_s(analise->cod, 1000, exp14->cod, strlen(exp14->cod));
				analise->ret = 1;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		analise->ret = 1;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *EXP12()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));

	if (tk == TKEBinario)
	{
		//strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		strncat_s(analise->cod, BUFSIZEINI, " e ", strlen(" e "));
		leToken();
		_ret *exp13 = EXP13();
		if (exp13->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, exp13->cod, strlen(exp13->cod));
			if (tk == TKEBinario)
			{
				strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				_ret *exp12 = EXP12();
				if (exp12->ret)
				{
					strncat_s(analise->cod, BUFSIZEINI, exp12->cod, strlen(exp12->cod));
					analise->ret = 1;
					return analise;

				}
				analise->ret = 0;
				return analise;
			}
			analise->ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *EXP11()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	
	_ret *exp13 = EXP13();
	if (exp13->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, exp13->cod, strlen(exp13->cod));
		if (tk == TKEBinario)
		{
			strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			_ret *exp12 = EXP12();
			if (exp12->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, exp12->cod, strlen(exp12->cod));
				analise->ret = 1;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		analise->ret = 1;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *EXP10()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKDivisao)
	{
		//strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *exp11 = EXP11();
		if (exp11->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, exp11->cod, strlen(exp11->cod));
			if (tk == TKDivisao)
			{
				strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				_ret *exp10 = EXP10();
				if (exp10->ret)
				{
					strncat_s(analise->cod, BUFSIZEINI, exp10->cod, strlen(exp10->cod));
					analise->ret = 1;
					return analise;
				}
				analise->ret = 0;
				return analise;
			}
			analise->ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *EXP9()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	_ret *exp11 = EXP11();
	if (exp11->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, exp11->cod, strlen(exp11->cod));
		if (tk == TKDivisao)
		{
			strncat_s(analise->cod, BUFSIZEINI, " ", strlen(" "));
			strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			strncat_s(analise->cod, BUFSIZEINI, " ", strlen(" "));
			_ret *exp10 = EXP10();
			if (exp10->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, exp10->cod, strlen(exp10->cod));
				analise->ret = 1;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		analise->ret = 1;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *EXP8()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKMultiplicacao)
	{
		//strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *exp9 = EXP9();
		if (exp9->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, exp9->cod, strlen(exp9->cod));
			if (tk == TKMultiplicacao)
			{
			//	strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				_ret *exp8 = EXP8();
				if (exp8->ret)
				{
					strncat_s(analise->cod, BUFSIZEINI, exp8->cod, strlen(exp8->cod));
					analise->ret = 1;
					return analise;
				}
				analise->ret = 0;
				return analise;
			}
			analise->ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *EXP7()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	_ret *exp9 = EXP9();
	if (exp9->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, exp9->cod, strlen(exp9->cod));
		if (tk == TKMultiplicacao)
		{
			strncat_s(analise->cod, BUFSIZEINI, " ", strlen(" "));
			strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			strncat_s(analise->cod, BUFSIZEINI, " ", strlen(" "));
			_ret *exp8 = EXP8();
			if (exp8->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, exp8->cod, strlen(exp8->cod));
				analise->ret = 1;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		analise->ret = 1;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *EXP6()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKOuBinario)
	{
		//strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		strncat_s(analise->cod, BUFSIZEINI, " ou ", strlen(" ou "));
		leToken();
		_ret *exp7 = EXP7();
		if (exp7->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, exp7->cod, strlen(exp7->cod));
			if (tk == TKOuBinario)
			{
				//strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				strncat_s(analise->cod, BUFSIZEINI, " ou ", strlen(" ou "));
				_ret *exp6 = EXP6();
				if (exp6->ret)
				{
					strncat_s(analise->cod, BUFSIZEINI, exp6->cod, strlen(exp6->cod));
					analise->ret = 1;
					return analise;
				}
				analise->ret = 0;
				return analise;
			}
			analise->ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *EXP5()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	_ret *exp7 = EXP7();
	if (exp7->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, exp7->cod, strlen(exp7->cod));
		if (tk == TKOuBinario)
		{
		//	strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			strncat_s(analise->cod, BUFSIZEINI, " ou ", strlen(" ou "));
			_ret *exp6 = EXP6();
			if (exp6->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, exp6->cod, strlen(exp6->cod));
				analise->ret = 1;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		analise->ret = 1;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *EXP4()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKSubtracao)
	{
		//strncpy_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *exp5 = EXP5();
		if (exp5->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, exp5->cod, strlen(exp5->cod));
			if (tk == TKSubtracao)
			{
			strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			_ret *exp4 = EXP4();
				if (exp4->ret)
				{
					strncat_s(analise->cod, BUFSIZEINI, exp4->cod, strlen(exp4->cod));
					analise->ret = 1;
					return analise;
				}
				analise->ret = 0;
				return analise;
			}
			analise->ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *EXP3()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	_ret *exp5 = EXP5();
	if (exp5->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, exp5->cod, strlen(exp5->cod));
		if (tk == TKSubtracao)
		{
			strncat_s(analise->cod, BUFSIZEINI, " - ", strlen(" - "));
			_ret *exp4 = EXP4();
			if (exp4->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, exp4->cod, strlen(exp4->cod));
				analise->ret = 1;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		analise->ret = 1;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *EXP2()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKSoma)
	{
		leToken();
		_ret *exp3 = EXP3();
		if (exp3->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, exp3->cod, strlen(exp3->cod));
			if (tk == TKSoma)
			{
				strncat_s(analise->cod, BUFSIZEINI, " + ", strlen(" + "));
				_ret *exp2 = EXP2();
				if (exp2->ret)
				{
					strncat_s(analise->cod, BUFSIZEINI, exp2->cod, strlen(exp2->cod));
					analise->ret = 1;
					return analise;
				}
				analise->ret = 0;
				return analise;
			}
			analise->ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *EXP1()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	_ret *exp3 = EXP3(); 
	if (exp3->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, exp3->cod, strlen(exp3->cod));
		if (tk == TKSoma)
		{
			strncat_s(analise->cod, BUFSIZEINI, " + ", strlen(" + "));
			_ret *exp2 = EXP2();
			if (exp2->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, exp2->cod, strlen(exp2->cod));
				analise->ret = 1;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		analise->ret = 1;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *COMP5()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKAbrePar)
	{
		strncpy_s(analise->cod, 1000, "(", strlen("("));
		leToken();
		_ret *comp0 = COMP0();
		if (comp0->ret){
			strncat_s(analise->cod, BUFSIZEINI, comp0->cod, strlen(comp0->cod));
			if (tk == TKFechaPar)
			{
				strncat_s(analise->cod, BUFSIZEINI, ")", strlen(")"));
				leToken();
				analise->ret = 1;
				return analise;
			}
			erroFechaPar();
			analise->ret = 0;
			return analise;
		}
		analise->ret = 0;
		return analise;
	}
	_ret *exp1 = EXP1();
	if (exp1->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, exp1->cod, strlen(exp1->cod));
		if (tk == TKMaior || tk == TKMaiorIgual || tk == TKMenor || tk == TKMenorIgual ||
			tk == TKIgual || tk == TKDiferente)
		{
			if (tk == TKIgual) {
				strncat_s(analise->cod, BUFSIZEINI, " = ", strlen(" = "));
			} else	if (tk == TKDiferente) {
				strncat_s(analise->cod, BUFSIZEINI, " <> ", strlen(" <> "));
			} else {
				strncat_s(analise->cod, BUFSIZEINI, " ", strlen(" "));
				strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				strncat_s(analise->cod, BUFSIZEINI, " ", strlen(" "));
			}
			leToken();

			_ret *exp1_1 = EXP1();
			if (exp1_1->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, exp1_1->cod, strlen(exp1_1->cod));
				analise->ret = 1;
				return analise;
			}
			erroExpInvalida();
			analise->ret = 0;
			return analise;
		}
		analise->ret = 1;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *COMP4()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKNegacao)
	{
	//	strncpy_s(analise->cod, 1000, "-", strlen("-"));
		strncpy_s(analise->cod, 1000, "nao", strlen("nao"));
		leToken();
		_ret *comp5 = COMP5();
		if (comp5->ret)
		{
			analise->ret = 1;
			strncat_s(analise->cod, BUFSIZEINI, comp5->cod, strlen(comp5->cod));
			return analise;
		}
		erroExpInvalida();
		analise->ret = 0;
		return analise;
	} else {
		_ret *comp5 = COMP5();
		if (comp5->ret) {
			strncat_s(analise->cod, BUFSIZEINI, comp5->cod, strlen(comp5->cod));
			analise->ret = 1;
			return analise;
		}
		else
		{
			analise->ret = 0;
			return analise;
		}
	}
	/*else if (COMP5())
	{
		//strncat_s(analise->cod,1000, comp5->cod, sizeof(comp5->cod));
		analise->ret = 1;
		return analise;
	}
	else
	{
		analise->ret = 0;
		return analise;
	}*/
}

_ret *COMP3()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKELogico)
	{
		//strncat_s(analise->cod,1000, "&", sizeof("&"));
		//strncat_s(analise->cod,1000, " e ", sizeof(" e "));
		leToken();
		
		_ret *comp4 = COMP4();
		if (comp4->ret)
		{
			strncat_s(analise->cod,1000, comp4->cod, strlen(comp4->cod));
			if (tk == TKELogico)
			{
		//		strncat_s(analise->cod,1000, "&", sizeof("&"));
				_ret *comp3 = COMP3();
				if (comp3->ret)
				{
					strncat_s(analise->cod, BUFSIZEINI, comp3->cod, strlen(comp3->cod));
					analise->ret = 1;
					return analise;
				}
				analise->ret = 0;
				return analise;
			}
			analise->ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise->ret = 0;
		return analise;
	}
	else
	{
		analise->ret = 0;
		return analise;		
	}
}

_ret *COMP2()
{

	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	_ret *comp4 = COMP4();
	if (comp4->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, comp4->cod, strlen(comp4->cod));
		if (tk == TKELogico)
		{
			//strncat_s(analise->cod,1000, "&", sizeof("&"));
			strncat_s(analise->cod, BUFSIZEINI, " e ", strlen(" e "));
			_ret *comp3 = COMP3();
			if (comp3->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, comp3->cod, strlen(comp3->cod));
				analise->ret = 1;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		analise->ret = 1;
		return analise;
	}
	else
	{
		analise->ret = 0;
		return analise;
	}
}

_ret *COMP1()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKOuLogico)
	{
		//strncpy_s(analise->cod,1000,"|",sizeof("|"));
		//strncpy_s(analise->cod,1000," ou ",sizeof(" ou "));
		strncat_s(analise->cod, BUFSIZEINI, " ou ", strlen(" ou "));
		leToken();
		_ret *comp2 = COMP2();
		if (comp2->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, comp2->cod, strlen(comp2->cod));
			if (tk == TKOuLogico)
			{
				//strncat_s(analise->cod,1000, "|", sizeof("|"));
				_ret *comp1 = COMP1();
				if (comp1->ret)
				{
					strncat_s(analise->cod, BUFSIZEINI, comp1->cod, strlen(comp1->cod));
					analise->ret = 1;
					return analise;
				}
				analise->ret = 0;
				return analise;
			}
			analise->ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise->ret = 0;
		return analise;

	}
	else
	{
		analise->ret = 0;
		return analise;
	}
}

_ret *COMP0()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	_ret *comp2 = COMP2();
	if (comp2->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, comp2->cod, strlen(comp2->cod));
		if (tk == TKOuLogico)
		{
			//strncat_s(analise->cod, BUFSIZEINI, " ou ", strlen(" ou "));
			_ret *comp1 = COMP1();
			if (comp1->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, comp1->cod, strlen(comp1->cod));
				analise->ret = 1;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		analise->ret = 1;
		return analise;
	}
	else
	{
		analise->ret = 0;
		return analise;
	}
}

_ret *ELSE(int nivel)
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKElse)
	{
		for (int i = 0; i < nivel; i++) {
			strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
		}
		strncat_s(analise->cod, 1000, "SENAO\r\n", strlen("SENAO\r\n"));//else
		leToken();
		int nivel2 = nivel + 1;
		_ret *bloco = BLOCO(nivel2);
		if (bloco->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, bloco->cod, strlen(bloco->cod));
			analise->ret = 1;
			return analise;
		}
	}
	else if (tk == TKElseIf)
	{
		for (int i = 0; i < nivel; i++) {
			strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
		}
		//strncat_s(analise->cod, BUFSIZEINI, "SENAO SE ", strlen("SENAO SE "));//elseif
		strncat_s(analise->cod, BUFSIZEINI, "SENAO\r\n", strlen("SENAO\r\n"));//elseif
		int nivel2 = nivel + 1;
		for (int i = 0; i < nivel2; i++) {
			strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
		}
		strncat_s(analise->cod, BUFSIZEINI, "SE ", strlen("SE "));//elseif
		leToken();
		if (tk == TKAbrePar)
		{
			strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			leToken();
			_ret *comp0 = COMP0();
			if (comp0->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, comp0->cod, strlen(comp0->cod));
				if (tk == TKFechaPar)
				{
					/*for (int i = 0; i < nivel; i++) {
						strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
					}*/
					strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
					strncat_s(analise->cod, BUFSIZEINI, " ENTAO\r\n", strlen(" ENTAO\r\n"));
					leToken();
					////strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
					////leToken();
					int nivel3 = nivel2 + 1;
					_ret *bloco = BLOCO(nivel3);
					if (bloco->ret)
					{
						strncat_s(analise->cod, BUFSIZEINI, bloco->cod, strlen(bloco->cod));
						if (tk == TKElse || tk == TKElseIf)
						{
							/*for (int i = 0; i < nivel2; i++) {
								strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
							}*/
							/*if (tk == TKElse) {
								strncat_s(analise->cod, BUFSIZEINI, "SENAO ", strlen("SENAO "));
							} else {
								strncat_s(analise->cod, BUFSIZEINI, "SENAO SE ", strlen("SENAO SE "));
							}*/
							_ret *elsee = ELSE(nivel2);
							if (elsee->ret)
							{
								strncat_s(analise->cod, BUFSIZEINI, elsee->cod, strlen(elsee->cod));
								for (int i = 0; i < nivel2; i++) {
									strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
								}
								strncat_s(analise->cod, BUFSIZEINI, "FIMSE\r\n", strlen("FIMSE\r\n"));
								analise->ret = 1;
								return analise;
							}

							/*adicionei TKEnd no elseif mas ainda não descobri como manipular os blocos.. entao 
							* ele está pegando o end do do elseif mas tratando ainda no bloco do nivel superior (if)

							*/
							if (tk == TKEnd)
							{
								for (int i = 0; i < nivel; i++) {
									strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
								}
								strncat_s(analise->cod, BUFSIZEINI, "FIMSE\r\n", strlen("FIMSE\r\n"));
								leToken();
								analise->ret = 1;
								return analise;
							}
							erroEnd();
							//analise->ret = 0;
							//return analise;
							/*
							//editei até aqui
							*/

							analise->ret = 0;
							return analise;
						}
						analise->ret = 1;
						return analise;
					}
					analise->ret = 0;
					return analise;
				}
				erroFechaPar();
				analise->ret = 0;
				return analise;
			}
			erroAbrePar();
			analise->ret = 0;
			return analise;
		}
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *IF(int nivel)
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKIf)
	{
		//strncat_s(analise->cod, BUFSIZEINI, "\r\n", strlen("\r\n"));
		
		for (int i = 0; i < nivel; i++) {
			strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
		}
		//strncpy_s(analise->cod, 1000, "\r\nSE ", strlen("\r\nSE "));
		strncat_s(analise->cod, BUFSIZEINI, "SE ", strlen("SE "));
		leToken();
		if (tk == TKAbrePar)
		{
			strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			leToken();
			_ret *comp0 = COMP0();
			if (comp0->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, comp0->cod, strlen(comp0->cod));
				if (tk == TKFechaPar)
				{
					strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
					/*strncat_s(analise->cod, BUFSIZEINI, "\r\n", strlen("\r\n"));
					//int nivel4 = nivel + 1;
					nivel = nivel + 1;
					for (int i = 0; i < nivel; i++) {
						strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
					}*/
					strncat_s(analise->cod, BUFSIZEINI, " ENTAO\r\n", strlen(" ENTAO\r\n"));
					leToken();
					int nivel2 = nivel + 1;
					_ret *bloco = BLOCO(nivel2);
					if (bloco->ret)
					{
						strncat_s(analise->cod, BUFSIZEINI, bloco->cod, strlen(bloco->cod));
						if (tk == TKElse || tk == TKElseIf)
						{
							/*
							if (tk == TKElseIf) {
								strncat_s(analise->cod,1000, "SENAO SE", sizeof("SENAO SE"));
							} else {
								strncat_s(analise->cod,1000, "SENAO", sizeof("SENAO"));
							}
							*/

							_ret *elsee = ELSE(nivel);
							if (!elsee->ret)
							{
								analise->ret = 0;
								return analise;
							} else {
								strncat_s(analise->cod, BUFSIZEINI, elsee->cod, strlen(elsee->cod));
							}
						}
						if (tk == TKEnd)
						{
							for (int i = 0; i < nivel; i++) {
								strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
							}
							//strncat_s(analise->cod, BUFSIZEINI, "FIMSE\r\n", strlen("FIMSE\r\n"));
							strncat_s(analise->cod, BUFSIZEINI, "FIMSE", strlen("FIMSE"));
							leToken();
							analise->ret = 1;
							return analise;
						}
						erroEnd();
						analise->ret = 0;
						return analise;
					}
					analise->ret = 0;
					return analise;
				}
				erroFechaPar();
				analise->ret = 0;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		erroAbrePar();
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *TRY(int nivel)
{	
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKTry)
	{
		for (int i = 0; i < nivel; i++) {
			strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
		}
		//strncpy_s(analise->cod, 1000, "TESTAR ", strlen("TESTAR"));
		strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		strncat_s(analise->cod, BUFSIZEINI, "\r\n", strlen("\r\n"));
		
		leToken();
		int nivel2 = nivel + 1;
		_ret *bloco = BLOCO(nivel2);
		if (bloco->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, bloco->cod, strlen(bloco->cod));
			if (tk == TKCatch)
			{
				for (int i = 0; i < nivel; i++) {
					strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
				}
				//strncat_s(analise->cod, BUFSIZEINI, "CATCH E", strlen("CATCH E"));
				strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				strncat_s(analise->cod, BUFSIZEINI, "\r\n", strlen("\r\n"));
		
				leToken();
				int nivel3 = nivel2 + 1;
				_ret *bloco2 = BLOCO(nivel2);
				if (bloco2->ret)
				{
					strncat_s(analise->cod, BUFSIZEINI, bloco2->cod, strlen(bloco2->cod));
					if (tk == TKEnd)
					{
						for (int i = 0; i < nivel; i++) {
							strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
						}	
						//strncat_s(analise->cod, BUFSIZEINI, "FIM CATCH", strlen("FIM CATCH"));
						strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
						strncat_s(analise->cod, BUFSIZEINI, "\r\n", strlen("\r\n"));
						leToken();
						analise->ret = 1;
						return analise;
					}
					erroEnd();
					analise->ret = 0;
					return analise;
				}
				analise->ret = 0;
				return analise;
			}
			msgErro("CATCH faltando!");
			analise->ret = 0;
			return analise;
		}
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *CRIAFUNCTION(int nivel)
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKFunction)
	{
		for (int i = 0; i < nivel; i++) {
			strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
		}
		strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		int nivel2 = nivel + 1;
		_ret *function = FUNCTION(nivel2);
		if (function->ret)
		{
			for (int i = 0; i < nivel; i++) {
				strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
			}
			strncat_s(analise->cod, BUFSIZEINI, function->cod, strlen(function->cod));
			int nivel3 = nivel2 + 1;
			_ret *bloco = BLOCO(nivel3);
			if (bloco->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, bloco->cod, strlen(bloco->cod));
				if (tk == TKEnd)
				{
					for (int i = 0; i < nivel; i++) {
						strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
					}
					strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
					leToken();
					analise->ret = 1;
					return analise;
				}
				erroEnd();
				analise->ret = 0;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *PARAM2()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	_ret *val = VAL();
	if (val->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, val->cod, strlen(val->cod));
		analise->ret = 1;
		return analise;
	}
	erroVal();
	analise->ret = 0;
	return analise;
}

_ret *PARAM1()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKVirgula)
	{
		//strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *param2 = PARAM2();
		if (param2->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, param2->cod, strlen(param2->cod));
			if (tk == TKVirgula)
			{
				strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				_ret *param1 = PARAM1();
				if (param1->ret)
				{
					analise->ret = 1;
					return analise;
				}
				analise->ret = 0;
				return analise;
			}
			analise->ret = 1;
			return analise;
		}
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *PARAM0()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	_ret *param2 = PARAM2();
	if (param2->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, param2->cod, strlen(param2->cod));
		if (tk == TKVirgula)
		{
			strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			_ret *param1 = PARAM1();
			if (param1->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, param1->cod, strlen(param1->cod));
				analise->ret = 1;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		analise->ret = 1;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *FUNCTION(int nivel)
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));

	_ret *ident = id(0);
	if (ident->ret)
	{
		for (int i = 0; i < nivel; i++) {
			strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
		}
	
		strncat_s(analise->cod, 1000, ident->cod, strlen(ident->cod));
		if (tk == TKAbrePar)
		{
			strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			leToken();
			_ret *param0 = PARAM0();
			if (param0->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, param0->cod, strlen(param0->cod));
				if (tk == TKFechaPar)
				{
					strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
					leToken();
					if (tk != 18) {
						strncat_s(analise->cod, BUFSIZEINI, "\r\n", strlen("\r\n"));
					}
					analise->ret = 1;
					return analise;
				}
				erroFechaPar();
				analise->ret = 0;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *PARFOR(int nivel)
{
	//Troca na analise lexica e sintatica
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKParfor)
	{
		for (int i = 0; i < nivel; i++) {
			strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
		}
		strncat_s(analise->cod, BUFSIZEINI, "PARA ", strlen("PARA "));
		leToken();
		_ret *atrib = ATRIB(nivel, 1);
		if (atrib->ret)
		{
			//strncat_s(analise->cod, BUFSIZEINI, " de ", strlen(" de "));
			strncat_s(analise->cod, BUFSIZEINI, atrib->cod, strlen(atrib->cod));
			if (tk == TKDoisPontos)
			{
			//	strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				//strncat_s(analise->cod, BUFSIZEINI, " ate ", strlen(" ate "));
				leToken();
				/*if (tk == TKDoisPontos) {
					strncat_s(analise->cod, BUFSIZEINI, " passo ", strlen(" passo "));
				} else {
					strncat_s(analise->cod, BUFSIZEINI, " ate ", strlen(" ate "));
				}*/
				_ret *val = VAL();
				if (val->ret)
				{
					if (tk == TKDoisPontos)
					{
						//strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
						strncat_s(analise->cod, BUFSIZEINI, " passo ", strlen(" passo "));
						strncat_s(analise->cod, BUFSIZEINI, val->cod, strlen(val->cod));
						leToken();
						_ret *val2 = VAL();
						if (!val2->ret)
						{
							erroVal();
							analise->ret = 0;
							return analise;
						} else {
							strncat_s(analise->cod, BUFSIZEINI, " ate ", strlen(" ate "));
							strncat_s(analise->cod, BUFSIZEINI, val2->cod, strlen(val2->cod));
							strncat_s(analise->cod, BUFSIZEINI, " FACA\r\n", strlen(" FACA\r\n"));
						}
					} else {
						strncat_s(analise->cod, BUFSIZEINI, " ate ", strlen(" ate "));
						strncat_s(analise->cod, BUFSIZEINI, val->cod, strlen(val->cod));
						strncat_s(analise->cod, BUFSIZEINI, " FACA\r\n", strlen(" FACA\r\n"));
					}
					int nivel2 = nivel + 1;
					_ret *bloco = BLOCO(nivel2);
					if (bloco->ret)
					{
						strncat_s(analise->cod, BUFSIZEINI, bloco->cod, strlen(bloco->cod));
						if (tk == TKEnd)
						{
							//strncat_s(analise->cod,1000, tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
							for (int i = 0; i < nivel; i++) {
								strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
							}
							strncat_s(analise->cod, BUFSIZEINI, "FIMPARA\r\n", strlen("FIMPARA\r\n"));
							leToken();
							analise->ret = 1;
							return analise;
						}
						erroEnd();
						analise->ret = 0;
						return analise;
					}
					analise->ret = 0;
					return analise;
				}
				erroVal();
				analise->ret = 0;
				return analise;
			}
			erroDoisPt();
			analise->ret = 0;
			return analise;
		}
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *CASEVALUE2()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	_ret *cons = cte();
	if (cons->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, cons->cod, strlen(cons->cod));
		if (tk == TKVirgula)
		{
			strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			leToken();
			_ret *casevalue2 = CASEVALUE2();
			if (casevalue2->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, casevalue2->cod, strlen(casevalue2->cod));
				analise->ret = 1;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		analise->ret = 1;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *CASEVALUE1()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	_ret *cons = cte();
	if (cons->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, cons->cod, strlen(cons->cod));
		if (tk == TKVirgula)
		{
			strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			leToken();
			_ret *casevalue2 = CASEVALUE2();
			if (casevalue2->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, casevalue2->cod, strlen(casevalue2->cod));
				analise->ret = 1;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *CASEVALUE0()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));

	//Alteração na gramatica
	if (tk == TKSubtracao) {
		strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
	}

	_ret *cons = cte();
	if (cons->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, cons->cod, strlen(cons->cod));
		analise->ret = 1;
		return analise;
	}
	else if (tk == TKAbreChave)
	{
		strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *casevalue1 = CASEVALUE1();
		if (casevalue1->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, cons->cod, strlen(casevalue1->cod));
			if (tk == TKFechaChave)
			{
				strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				leToken();
				analise->ret = 1;
				return analise;
			}
			msgErro("Falta fechar as chaves");
			analise->ret = 0;
			return analise;
		}
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *CASE(int nivel, char variavel[])
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKCase)
	{
		//strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		for (int i = 0; i < nivel; i++) {
			strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
		}
		//strncat_s(analise->cod, BUFSIZEINI, "CASO ", strlen("CASO "));
		strncat_s(analise->cod, BUFSIZEINI, "SE (", strlen("SE( "));
		leToken();
		_ret *casevalue0 = CASEVALUE0();
		if (casevalue0->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, variavel, strlen(variavel));
			strncat_s(analise->cod, BUFSIZEINI, " = ", strlen(" = "));
			strncat_s(analise->cod, BUFSIZEINI, casevalue0->cod, strlen(casevalue0->cod));
			strncat_s(analise->cod, BUFSIZEINI, ") ENTAO ", strlen(") ENTAO "));
			
			
			
			strncat_s(analise->cod, BUFSIZEINI, "\r\n", strlen("\r\n"));

			int nivel2 = nivel + 1;
			_ret *bloco = BLOCO(nivel2);
			if (bloco->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, bloco->cod, strlen(bloco->cod));
				if (tk == TKCase || tk == TKOtherwise)
				{
					int ehDefault = 0;
					if (tk == TKOtherwise) {
						ehDefault = 1;
					}
					for (int i = 0; i < nivel; i++) {
						strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
					}

					strncat_s(analise->cod, BUFSIZEINI, "SENAO\r\n ", strlen("SENAO\r\n "));
					nivel = nivel + 1;
					/*for (int i = 0; i < nivel; i++) {
						strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
					}*/
					/*if (tk == TKOtherwise) {
						strncat_s(analise->cod, BUFSIZEINI, "OUTROCASO ", strlen("OUTROCASO "));
					} else {
						strncat_s(analise->cod, BUFSIZEINI, "CASO ", strlen("CASO "));
					}*/
					//nivel = nivel + 1;
					_ret *casee = CASE(nivel, variavel);
					if (casee->ret)
					{
						strncat_s(analise->cod, BUFSIZEINI, casee->cod, strlen(casee->cod));
						if (ehDefault == 0) {
							for (int i = 0; i < nivel; i++) {
								strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
							}
							strncat_s(analise->cod, BUFSIZEINI, "FIMSE\r\n", strlen("FIMSE\r\n"));
						}
						analise->ret = 1;
						return analise;
					}
					analise->ret = 0;
					return analise;
				}
				analise->ret = 1;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		analise->ret = 0;
		return analise;
	}
	else if (tk == TKOtherwise)
	{
		//strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		//strncat_s(analise->cod, BUFSIZEINI, "OUTROCASO\r\n", strlen("OUTROCASO\r\n"));
		//strncat_s(analise->cod, BUFSIZEINI, "SENAO\r\n", strlen("SENAO\r\n"));
		leToken();
		int nivel2 = nivel + 1;
		_ret *bloco = BLOCO(nivel);
		if (bloco->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, bloco->cod, strlen(bloco->cod));
			analise->ret = 1;
			return analise;
		}
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *SWITCH(int nivel)
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKSwitch)
	{
		//strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		/*for (int i = 0; i < nivel; i++) {
				strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
		}*/
		
		//strncat_s(analise->cod, BUFSIZEINI, "ESCOLHA ", strlen("ESCOLHA "));
		//strncat_s(analise->cod, BUFSIZEINI, "SE ", strlen("SE "));
		leToken();
		_ret *ident = id(1);
		if (ident->ret)
		{
			/*strncat_s(analise->cod, BUFSIZEINI, ident->cod, strlen(ident->cod));
			int nivel2 = nivel + 1;
			strncat_s(analise->cod, BUFSIZEINI, "\r\n", strlen("\r\n"));
			for (int i = 0; i < nivel2; i++) {
				strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
			}*/
			_ret *casee = CASE(nivel, ident->cod);
			if (casee->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, casee->cod, strlen(casee->cod));
				if (tk == TKEnd)
				{
					for (int i = 0; i < nivel; i++) {
						strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
					}
					//strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
					strncat_s(analise->cod, BUFSIZEINI, "FIMSE", strlen("FIMSE"));

					leToken();
					analise->ret = 1;
					return analise;
				}
				erroEnd();
				analise->ret = 0;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *WHILE(int nivel)
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKWhile)
	{
		for (int i = 0; i < nivel; i++) {
			strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
		}
		strncat_s(analise->cod, BUFSIZEINI, "ENQUANTO ", strlen("ENQUANTO "));
		leToken();
		if (tk == TKAbrePar)
		{
			strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			leToken();
			_ret *comp0 = COMP0();
			if (comp0->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, comp0->cod, strlen(comp0->cod));
				if (tk == TKFechaPar)
				{
					strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
					strncat_s(analise->cod, BUFSIZEINI, " FACA\r\n", strlen(" FACA\r\n"));
					leToken();
					int nivel2 = nivel + 1;
					_ret *bloco = BLOCO(nivel2);
					if (bloco->ret)
						strncat_s(analise->cod, BUFSIZEINI, bloco->cod, strlen(bloco->cod));
					{
						if (tk == TKEnd)
						{
							for (int i = 0; i < nivel; i++) {
								strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
							}
							//strncat_s(analise->cod,1000, tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
							strncat_s(analise->cod, BUFSIZEINI, "FIMENQUANTO\r\n", strlen("FIMENQUANTO\r\n"));
							leToken();
							analise->ret = 1;
							return analise;
						}
						erroEnd();
						analise->ret = 0;
						return analise;
					}
					analise->ret = 0;
					return analise;
				}
				erroFechaPar();
				analise->ret = 0;
				return analise;
			}
			analise->ret = 0;
			return analise;
		} else {
			//Analise lexica adicionada
			_ret *comp0 = COMP0();
			if (comp0->ret)
			{
				strncat_s(analise->cod, BUFSIZEINI, comp0->cod, strlen(comp0->cod));
				strncat_s(analise->cod, BUFSIZEINI, " FACA\r\n", strlen(" FACA\r\n"));
				//leToken();
				int nivel2 = nivel + 1;
				_ret *bloco = BLOCO(nivel2);
				if (bloco->ret)
					strncat_s(analise->cod, BUFSIZEINI, bloco->cod, strlen(bloco->cod));
				{
					if (tk == TKEnd)
					{
						for (int i = 0; i < nivel; i++) {
							strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
						}
						//strncat_s(analise->cod,1000, tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
						strncat_s(analise->cod, BUFSIZEINI, "FIMENQUANTO\r\n", strlen("FIMENQUANTO\r\n"));
						leToken();
						analise->ret = 1;
						return analise;
					}
					erroFechaPar();
					analise->ret = 0;
					return analise;
				}
				erroEnd();
				analise->ret = 0;
				return analise;
			}
			analise->ret = 0;
			return analise;
		}
		erroAbrePar();
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *FOR(int nivel)
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKFor)
	{
		for (int i = 0; i < nivel; i++) {
			strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
		}
		strncat_s(analise->cod, BUFSIZEINI, "PARA ", strlen("PARA "));
		leToken();
		_ret *atrib = ATRIB(nivel, 1);
		if (atrib->ret)
		{
			//strncat_s(analise->cod, BUFSIZEINI, " de ", strlen(" de "));
			strncat_s(analise->cod, BUFSIZEINI, atrib->cod, strlen(atrib->cod));
			if (tk == TKDoisPontos)
			{
			//	strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				//strncat_s(analise->cod, BUFSIZEINI, " ate ", strlen(" ate "));
				leToken();
				/*if (tk == TKDoisPontos) {
					strncat_s(analise->cod, BUFSIZEINI, " passo ", strlen(" passo "));
				} else {
					strncat_s(analise->cod, BUFSIZEINI, " ate ", strlen(" ate "));
				}*/
				_ret *val = VAL();
				if (val->ret)
				{
					if (tk == TKDoisPontos)
					{
						//strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
						strncat_s(analise->cod, BUFSIZEINI, " passo ", strlen(" passo "));
						strncat_s(analise->cod, BUFSIZEINI, val->cod, strlen(val->cod));
						leToken();
						_ret *val2 = VAL();
						if (!val2->ret)
						{
							erroVal();
							analise->ret = 0;
							return analise;
						} else {
							strncat_s(analise->cod, BUFSIZEINI, " ate ", strlen(" ate "));
							strncat_s(analise->cod, BUFSIZEINI, val2->cod, strlen(val2->cod));
							strncat_s(analise->cod, BUFSIZEINI, " FACA\r\n", strlen(" FACA\r\n"));
						}
					} else {
						strncat_s(analise->cod, BUFSIZEINI, " ate ", strlen(" ate "));
						strncat_s(analise->cod, BUFSIZEINI, val->cod, strlen(val->cod));
						strncat_s(analise->cod, BUFSIZEINI, " FACA\r\n", strlen(" FACA\r\n"));
					}
					int nivel2 = nivel + 1;
					_ret *bloco = BLOCO(nivel2);
					if (bloco->ret)
					{
						strncat_s(analise->cod, BUFSIZEINI, bloco->cod, strlen(bloco->cod));
						if (tk == TKEnd)
						{
							//strncat_s(analise->cod,1000, tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
							for (int i = 0; i < nivel; i++) {
								strncat_s(analise->cod, BUFSIZEINI, "\t", strlen("\t"));
							}
							strncat_s(analise->cod, BUFSIZEINI, "FIMPARA\r\n", strlen("FIMPARA\r\n"));
							leToken();
							analise->ret = 1;
							return analise;
						}
						erroEnd();
						analise->ret = 0;
						return analise;
					}
					analise->ret = 0;
					return analise;
				}
				erroVal();
				analise->ret = 0;
				return analise;
			}
			erroDoisPt();
			analise->ret = 0;
			return analise;
		}
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *VAL()
{
	int marcaPos = setPos();
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));

	_ret *func = FUNCTION(0);
	if (func->ret)
	{
		strncpy_s(analise->cod, 1000, func->cod, strlen(func->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);

	_ret *comp0 = COMP0();
	if (comp0->ret)
	{
		strncpy_s(analise->cod, 1000, comp0->cod, strlen(comp0->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *aid = id(1);
	if (aid->ret)
	{
		strncpy_s(analise->cod, 1000, aid->cod, strlen(aid->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *constan = cte();
	if (constan->ret)
	{
		strncpy_s(analise->cod, 1000, constan->cod, strlen(constan->cod));
		analise->ret = 1;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *COMANDO(int nivel)
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	int marcaPos = setPos();

	if (nivel > 0) {
		 int a;
		 a = 10;
	}
	_ret *atrib = ATRIB(nivel, 0);

	if (atrib->ret)
	{
		strncpy_s(analise->cod, 1000, atrib->cod, strlen(atrib->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *fora = FOR(nivel); 
	if (fora->ret)
	{
		strncpy_s(analise->cod, 1000, fora->cod, strlen(fora->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *whileC = WHILE(nivel);
	if (whileC->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, whileC->cod, strlen(whileC->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *switchC = SWITCH(nivel);
	if (switchC->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, switchC->cod, strlen(switchC->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *cmdIf = IF(nivel);
	if (cmdIf->ret)
	{
		strncpy_s(analise->cod,1000,cmdIf->cod,strlen(cmdIf->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *tryC = TRY(nivel);
	if (tryC->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, tryC->cod, strlen(tryC->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *parFor = PARFOR(nivel);
	if (parFor->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, parFor->cod, strlen(parFor->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *criaFunction = CRIAFUNCTION(nivel);
	if (criaFunction->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, criaFunction->cod, strlen(criaFunction->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *function = FUNCTION(nivel);
	if (function->ret)
	{
		strncat_s(analise->cod, BUFSIZEINI, function->cod, strlen(function->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	if (tk == TKBreak)
	{
		strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		analise->ret = 1;
		return analise;
	}
	else if (tk == TKContinue)
	{
		strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		analise->ret = 1;
		return analise;
	}
	else if (tk == TKReturn)
	{
		strncat_s(analise->cod, BUFSIZEINI, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *val = VAL();
		if (val->ret)
		{
			strncat_s(analise->cod, BUFSIZEINI, val->cod, strlen(val->cod));
			analise->ret = 1;
			return analise;
		}
		erroVal();
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *BLOCO(int nivel)
{

	_ret *analise_bl = (_ret*)malloc(sizeof(_ret) * 1);
	analise_bl->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise_bl->cod, 1000, "", strlen(""));
	_ret *comando = COMANDO(nivel);
	if (comando->ret)
	{
		/*for (int i = 0; i < nivel; i++) {
			strncat_s(analise_bl->cod, 1000, "\t", strlen("\t"));
		}*/
		//strncpy_s(analise_bl->cod, 1000, comando->cod, strlen(comando->cod));
		strncat_s(analise_bl->cod, 1000, comando->cod, strlen(comando->cod));
		if (tk == TKPontoeVirg)
		{
			strncat_s(analise_bl->cod, 1000, ";\r\n", strlen(";\r\n"));
			leToken();
		} else {
				strncat_s(analise_bl->cod, 1000, "\r\n", strlen("\r\n"));
		}
		if (tk == TKId || tk == TKFor || tk == TKWhile || tk == TKSwitch ||
			tk == TKIf || tk == TKTry || tk == TKParfor || tk == TKFunction ||
			tk == TKBreak || tk == TKContinue || tk == TKReturn)
		{
			//nivel = nivel +1;

	//		strncat_s(analise_bl->cod,1000, tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			_ret *bloco = BLOCO(nivel);
			if (bloco->ret)
			{
				strncat_s(analise_bl->cod, strlen(analise_bl->cod)+strlen(bloco->cod)+2, bloco->cod, strlen(bloco->cod));
				analise_bl->ret = 1;
				return analise_bl;
			}
			analise_bl->ret = 0;
			return analise_bl;
		}
		analise_bl->ret = 1;
		return analise_bl;
	}
	else
	{
		analise_bl->ret = 0;
		return analise_bl;
	}
}

/*void atribuirCod(_ret *vet, char string) {
	strncpy_s((*vet)->cod, string, sizeof(string));
//	strncpy_s((*vet)->cod, "teste", sizeof("teste"));
}*/

_ret *INICIO()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	//_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	_ret *tmp;

	//tmp = &analise;
	//tmp = (_ret *)malloc(sizeof(_ret)*1);
	//memset(tmp, 0, sizeof(_ret) * 1); 
	//strncpy_s(analise->cod,1000, "", sizeof(""));
	//analise->cod = (char *)malloc(sizeof(char) * 1000);
	// analise = (_ret *)malloc(sizeof(_ret));
	analise->cod = (char *)malloc(sizeof(char) * 4000);
	strncpy_s(analise->cod, 4000, "", strlen(""));
		//necessário para strncat (strncat não concatena strings sem possuir terminador nulo \0
		//ou strncpy_s ou string[strlen(string)] = '\0'; porém a última alternativa resulta em duas strings gigantes contactenadas
	
	_ret *bloco = BLOCO(1);
	// strncpy_s(bloco->cod, "", sizeof(""));
	//tmp = &bloco;
	//tmp = (_ret *)malloc(sizeof(_ret) * 3000);
	//memset(tmp, 0, sizeof(_ret) * 1);
	if (bloco->ret)
	{
		analise->ret = 1;
		strncat_s(analise->cod, BUFSIZEINI, "algoritmo 'compiladores'\r\n\r\n", strlen("algoritmo 'compiladores'\r\n\r\n"));
		strncat_s(analise->cod, BUFSIZEINI, "var\r\n\t", strlen("var\r\n\t"));
		int indice;
		for (indice = 0; indice < nroVariaveis; indice++) {
			if (indice > 0) {
				strncat_s(analise->cod, BUFSIZEINI, ", ", strlen(", "));
			}
			strncat_s(analise->cod, BUFSIZEINI, variaveis[indice], strlen(variaveis[indice]));
		}
		if (nroVariaveis > 0) {
			strncat_s(analise->cod, BUFSIZEINI, " : real\r\n", strlen(" : real\r\n"));
		}
		strncat_s(analise->cod, BUFSIZEINI, "\ninicio\r\n\r\n", strlen("inicio\r\n\r\n"));
		//char *tmp2;
		//tmp2 = (char *) &(analise->cod);
		//tmp2 = (char *) realloc(tmp,sizeof(char) * 3000);
		//memset(tmp, 0, sizeof(char) * 3000);
		//analise->cod = (char [250])realloc(analise->cod, sizeof(char) * 3000);
			//(char *) realloc(analise->cod, sizeof(char) * 3000);
		//analise = (_ret *) realloc(analise, sizeof(_ret) * 30);
		//char test[3000];
		//strncpy(analise->cod, test, sizeof(char) * 3000);
		//analise->cod = test;
		//analise->cod =(char *) malloc(strlen(analise->cod) + strlen(bloco->cod)+1);
		strncat_s((char *)analise->cod, (strlen(analise->cod) + strlen(bloco->cod)+2), 
			(char *)bloco->cod, strlen(bloco->cod));
		strncat_s(analise->cod, strlen(analise->cod) + strlen("\r\nfimalgoritmo") +1, "\r\nfimalgoritmo", strlen("\r\nfimalgoritmo"));
		fprintf(portugues, "%s", analise->cod);
		return analise;
	}
	else
	{
		analise->ret = 0;
		return analise;
	}
}

int main()
{
	int i = 0;
	char ch;
	characters = (char *)malloc(space);
	tokens = (struct token *)malloc(space);

	FILE * fp = fopen("Entrada.m", "r");

	s_ret = (_ret*) malloc(sizeof(_ret));
	if (fp == NULL)
	{
		printf("Erro ao abrir o arquivo de entrada (Entrada.m).\n");
		getchar();
		exit(1);
	}

	while ((ch = fgetc(fp)) != EOF)
	{
		characters[i] = ch;
		i++;
		space++;
		characters = (char *)realloc(characters, space);
	}

	characters[i] = '\0';

	newFile = fopen("Saida.lex", "wb+");

	if (newFile == NULL)
	{
		printf("Erro ao abrir o arquivo de saida (Saida.lex).\n");
		getchar();
		exit(1);
	}

	portugues = fopen("Portugues.txt", "wb+");

	if (portugues == NULL)
	{
		printf("Erro ao abrir o arquivo de saida (Portugues.txt).\n");
		getchar();
		exit(1);
	}


	leToken();
	while (tk != TKFim)
	{

		_ret *inicio = INICIO();

		if (!inicio->ret)
		{
			ocorreuErro();
			getchar();
			return 0;
		}
	//	fprintf(portugues, "%s \n", inicio->cod);
		leToken();
	}


	fclose(newFile);
	fclose(portugues);


	printf("Arquivo gerado com sucesso!");

	//getchar();
	//system("pause");
	return 0;
}