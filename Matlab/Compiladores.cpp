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
_ret *FUNCTION();
_ret *EXP1();
_ret *COMP0();
_ret *BLOCO();
_ret *VAL();

_ret *id()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKId)
	{
		strncat_s(analise->cod,1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		incluirTS(tokens[posTK].elemento);
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
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		analise->ret = 1;
		return analise;
	}
	if (tk == TKConstanteReal)
	{
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		analise->ret = 1;
		return analise;
	}
	if (tk == TKString)
	{
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		analise->ret = 1;
		return analise;
	}
	if (tk == TKTrue)
	{
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		analise->ret = 1;
		return analise;
	}
	if (tk == TKFalse)
	{
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		analise->ret = 1;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *ATRIB()
{

	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	_ret *ident = id();
	if (ident->ret)
	{
		strncat_s(analise->cod, 1000, ident->cod, strlen(ident->cod));
		if (tk == TKAtrib)
		{
			//strncat_s(analise->cod,1000, tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			strncat_s(analise->cod, 1000, "<-", strlen("<-"));
			leToken();
			_ret *val = VAL();
			if (val->ret)
			{
				analise->ret = 1;
				strncat_s(analise->cod, 1000, val->cod, strlen(val->cod));
				return analise;
			}
			erroVal();
			analise->ret = 0;
			return analise;
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
			strncat_s(analise->cod, 1000, exp1->cod, strlen(exp1->cod));
			if (tk == TKFechaPar)
			{
				strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
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
	_ret *ident = id();
	if (ident->ret)
	{
		strncat_s(analise->cod, 1000, ident->cod, strlen(ident->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *cons = cte();
	if (cons->ret)
	{
		strncat_s(analise->cod, 1000, cons->cod, strlen(cons->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *function = FUNCTION();
	if (function->ret)
	{
		strncat_s(analise->cod, 1000, function->cod, strlen(function->cod));
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
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *expFim = EXPFIM();
		if (expFim->ret)
		{
			strncat_s(analise->cod, 1000, expFim->cod, strlen(expFim->cod));
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
			strncat_s(analise->cod, 1000, expFim2->cod, strlen(expFim2->cod));
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
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *exp15 = EXP15();
		if (exp15->ret)
		{
			strncat_s(analise->cod, 1000, exp15->cod, strlen(exp15->cod));
			if (tk == TKPotencia)
			{
				strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
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
			strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			_ret *exp14 = EXP14();
			if (exp14->ret)
			{
				strncpy_s(analise->cod, 1000, exp14->cod, strlen(exp14->cod));
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
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *exp13 = EXP13();
		if (exp13->ret)
		{
			strncat_s(analise->cod, 1000, exp13->cod, strlen(exp13->cod));
			if (tk == TKEBinario)
			{
				strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				_ret *exp12 = EXP12();
				if (exp12->ret)
				{
					strncat_s(analise->cod, 1000, exp12->cod, strlen(exp12->cod));
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
		strncat_s(analise->cod, 1000, exp13->cod, strlen(exp13->cod));
		if (tk == TKEBinario)
		{
			strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			_ret *exp12 = EXP12();
			if (exp12->ret)
			{
				strncat_s(analise->cod, 1000, exp12->cod, strlen(exp12->cod));
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
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *exp11 = EXP11();
		if (exp11->ret)
		{
			strncat_s(analise->cod, 1000, exp11->cod, strlen(exp11->cod));
			if (tk == TKDivisao)
			{
				strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				_ret *exp10 = EXP10();
				if (exp10->ret)
				{
					strncat_s(analise->cod, 1000, exp10->cod, strlen(exp10->cod));
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
		strncat_s(analise->cod, 1000, exp11->cod, strlen(exp11->cod));
		if (tk == TKDivisao)
		{
			strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			_ret *exp10 = EXP10();
			if (exp10->ret)
			{
				strncat_s(analise->cod, 1000, exp10->cod, strlen(exp10->cod));
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
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *exp9 = EXP9();
		if (exp9->ret)
		{
			strncat_s(analise->cod, 1000, exp9->cod, strlen(exp9->cod));
			if (tk == TKMultiplicacao)
			{
				strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				_ret *exp8 = EXP8();
				if (exp8->ret)
				{
					strncat_s(analise->cod, 1000, exp8->cod, strlen(exp8->cod));
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
		strncat_s(analise->cod, 1000, exp9->cod, strlen(exp9->cod));
		if (tk == TKMultiplicacao)
		{
			strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			_ret *exp8 = EXP8();
			if (exp8->ret)
			{
				strncat_s(analise->cod, 1000, exp8->cod, strlen(exp8->cod));
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
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *exp7 = EXP7();
		if (exp7->ret)
		{
			strncat_s(analise->cod, 1000, exp7->cod, strlen(exp7->cod));
			if (tk == TKOuBinario)
			{
				strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				_ret *exp6 = EXP6();
				if (exp6->ret)
				{
					strncat_s(analise->cod, 1000, exp6->cod, strlen(exp6->cod));
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
		strncat_s(analise->cod, 1000, exp7->cod, strlen(exp7->cod));
		if (tk == TKOuBinario)
		{
			strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			_ret *exp6 = EXP6();
			if (exp6->ret)
			{
				strncat_s(analise->cod, 1000, exp6->cod, strlen(exp6->cod));
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
		strncpy_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *exp5 = EXP5();
		if (exp5->ret)
		{
			if (tk == TKSubtracao)
			{
				strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			_ret *exp4 = EXP4();
				if (exp4->ret)
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
		strncat_s(analise->cod, 1000, exp5->cod, strlen(exp5->cod));
		if (tk == TKSubtracao)
		{
			strncat_s(analise->cod, 1000, "-", strlen("-"));
			_ret *exp4 = EXP4();
			if (exp4->ret)
			{
				strncat_s(analise->cod, 1000, exp4->cod, strlen(exp4->cod));
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
			strncat_s(analise->cod, 1000, exp3->cod, strlen(exp3->cod));
			if (tk == TKSoma)
			{
				strncat_s(analise->cod, 1000, "+", strlen("+"));
				_ret *exp2 = EXP2();
				if (exp2->ret)
				{
					strncat_s(analise->cod, 1000, exp2->cod, strlen(exp2->cod));
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
		strncat_s(analise->cod, 1000, exp3->cod, strlen(exp3->cod));
		if (tk == TKSoma)
		{
			strncat_s(analise->cod, 1000, "+", strlen("+"));
			_ret *exp2 = EXP2();
			if (exp2->ret)
			{
				strncat_s(analise->cod, 1000, exp2->cod, strlen(exp2->cod));
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
			strncat_s(analise->cod, 1000, comp0->cod, strlen(comp0->cod));
			if (tk == TKFechaPar)
			{
				strncat_s(analise->cod, 1000, ")", strlen(")"));
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
		strncat_s(analise->cod, 1000, exp1->cod, strlen(exp1->cod));
		if (tk == TKMaior || tk == TKMaiorIgual || tk == TKMenor || tk == TKMenorIgual ||
			tk == TKIgual || tk == TKDiferente)
		{
			if (tk == TKIgual) {
				strncat_s(analise->cod, 1000, "=", strlen("="));
			} else {
				strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			}
			leToken();

			_ret *exp1_1 = EXP1();
			if (exp1_1->ret)
			{
				strncat_s(analise->cod, 1000, exp1_1->cod, strlen(exp1_1->cod));
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
		strncpy_s(analise->cod, 1000, "-", strlen("-"));
		leToken();
		_ret *comp5 = COMP5();
		if (comp5->ret)
		{
			analise->ret = 1;
			strncat_s(analise->cod, 1000, comp5->cod, strlen(comp5->cod));
			return analise;
		}
		erroExpInvalida();
		analise->ret = 0;
		return analise;
	} else {
		_ret *comp5 = COMP5();
		if (comp5->ret) {
			strncat_s(analise->cod, 1000, comp5->cod, strlen(comp5->cod));
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
					strncat_s(analise->cod, 1000, comp3->cod, strlen(comp3->cod));
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
		strncat_s(analise->cod, 1000, comp4->cod, strlen(comp4->cod));
		if (tk == TKELogico)
		{
			//strncat_s(analise->cod,1000, "&", sizeof("&"));
			strncat_s(analise->cod, 1000, " e ", strlen(" e "));
			_ret *comp3 = COMP3();
			if (comp3->ret)
			{
				strncat_s(analise->cod, 1000, comp3->cod, strlen(comp3->cod));
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
		leToken();
		_ret *comp2 = COMP2();
		if (comp2->ret)
		{
			strncat_s(analise->cod, 1000, comp2->cod, strlen(comp2->cod));
			if (tk == TKOuLogico)
			{
				//strncat_s(analise->cod,1000, "|", sizeof("|"));
				_ret *comp1 = COMP1();
				if (comp1->ret)
				{
					strncat_s(analise->cod, 1000, comp1->cod, strlen(comp1->cod));
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
		strncat_s(analise->cod, 1000, comp2->cod, strlen(comp2->cod));
		if (tk == TKOuLogico)
		{
			strncat_s(analise->cod, 1000, " ou ", strlen(" ou "));
			_ret *comp1 = COMP1();
			if (comp1->ret)
			{
				strncat_s(analise->cod, 1000, comp1->cod, strlen(comp1->cod));
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

_ret *ELSE()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKElse)
	{
		strncpy_s(analise->cod, 1000, "SENAO\n\t", strlen("SENAO\n\t"));
		leToken();
		_ret *bloco = BLOCO();
		if (bloco->ret)
		{
			strncat_s(analise->cod, 1000, bloco->cod, strlen(bloco->cod));
			analise->ret = 1;
			return analise;
		}
	}
	else if (tk == TKElseIf)
	{
		strncat_s(analise->cod, 1000, "SENAO SE\n\t", strlen("SENAO SE\n\t"));
		leToken();
		if (tk == TKAbrePar)
		{
			strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			leToken();
			_ret *comp0 = COMP0();
			if (comp0->ret)
			{
				strncat_s(analise->cod, 1000, comp0->cod, strlen(comp0->cod));
				if (tk == TKFechaPar)
				{
					strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
					leToken();
					_ret *bloco = BLOCO();
					if (bloco->ret)
					{
						strncat_s(analise->cod, 1000, bloco->cod, strlen(bloco->cod));
						if (tk == TKElse || tk == TKElseIf)
						{
							strncat_s(analise->cod, 1000, "SENAO SE VER", strlen("SENAO SE VER"));
							_ret *elsee = ELSE();
							if (elsee->ret)
							{
								strncat_s(analise->cod, 1000, elsee->cod, strlen(elsee->cod));
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

_ret *IF()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKIf)
	{
		strncpy_s(analise->cod, 1000, "\nSE ", strlen("\nSE "));
		leToken();
		if (tk == TKAbrePar)
		{
			strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			leToken();
			_ret *comp0 = COMP0();
			if (comp0->ret)
			{
				strncat_s(analise->cod, 1000, comp0->cod, strlen(comp0->cod));
				if (tk == TKFechaPar)
				{
					strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
					strncat_s(analise->cod, 1000, " ENTAO\n\t", strlen(" ENTAO\n\t"));
					leToken();
					_ret *bloco = BLOCO();
					if (bloco->ret)
					{
						strncat_s(analise->cod, 1000, bloco->cod, strlen(bloco->cod));
						if (tk == TKElse || tk == TKElseIf)
						{
							/*
							if (tk == TKElseIf) {
								strncat_s(analise->cod,1000, "SENAO SE", sizeof("SENAO SE"));
							} else {
								strncat_s(analise->cod,1000, "SENAO", sizeof("SENAO"));
							}
							*/

							_ret *elsee = ELSE();
							if (!elsee->ret)
							{
								analise->ret = 0;
								return analise;
							} else {
								strncat_s(analise->cod, 1000, elsee->cod, strlen(elsee->cod));
							}
						}
						if (tk == TKEnd)
						{
							strncat_s(analise->cod, 1000, "FIMSE\n", strlen("FIMSE\n"));
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

_ret *TRY()
{	
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKTry)
	{
		strncpy_s(analise->cod, 1000, "TESTAR ", strlen("TESTAR"));
		leToken();
		_ret *bloco = BLOCO();
		if (bloco->ret)
		{
			strncat_s(analise->cod, 1000, bloco->cod, strlen(bloco->cod));
			if (tk == TKCatch)
			{
				strncat_s(analise->cod, 1000, "CATCH E", strlen("CATCH E"));
				leToken();
				_ret *bloco2 = BLOCO();
				if (bloco2->ret)
				{
					strncat_s(analise->cod, 1000, bloco2->cod, strlen(bloco2->cod));
					if (tk == TKEnd)
					{
						strncat_s(analise->cod, 1000, "FIM CATCH", strlen("FIM CATCH"));
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

_ret *CRIAFUNCTION()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKFunction)
	{
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *function = FUNCTION();
		if (function->ret)
		{
			strncat_s(analise->cod, 1000, function->cod, strlen(function->cod));
			_ret *bloco = BLOCO();
			if (bloco->ret)
			{
				strncat_s(analise->cod, 1000, bloco->cod, strlen(bloco->cod));
				if (tk == TKEnd)
				{
					strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
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
		strncat_s(analise->cod, 1000, val->cod, strlen(val->cod));
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
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *param2 = PARAM2();
		if (param2->ret)
		{
			strncat_s(analise->cod, 1000, param2->cod, strlen(param2->cod));
			if (tk == TKVirgula)
			{
				strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
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
		strncat_s(analise->cod, 1000, param2->cod, strlen(param2->cod));
		if (tk == TKVirgula)
		{
			strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			_ret *param1 = PARAM1();
			if (param1->ret)
			{
				strncat_s(analise->cod, 1000, param1->cod, strlen(param1->cod));
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

_ret *FUNCTION()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));

	_ret *ident = id();
	if (ident->ret)
	{
		strncpy_s(analise->cod, 1000, ident->cod, strlen(ident->cod));
		if (tk == TKAbrePar)
		{
			strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			leToken();
			_ret *param0 = PARAM0();
			if (param0->ret)
			{
				strncat_s(analise->cod, 1000, param0->cod, strlen(param0->cod));
				if (tk == TKFechaPar)
				{
					strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
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
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *PARFOR()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	_ret *atrib = ATRIB();
	if (atrib->ret)
	{
		strncat_s(analise->cod, 1000, atrib->cod, strlen(atrib->cod));
		if (tk == TKDoisPontos)
		{
			strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			leToken();
			_ret *val = VAL();
			if (val->ret)
			{
				strncat_s(analise->cod, 1000, val->cod, strlen(val->cod));
				if (tk == TKPontoeVirg)
				{
					strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
					leToken();
					_ret *bloco = BLOCO();
					if (bloco->ret)
					{
						strncat_s(analise->cod, 1000, bloco->cod, strlen(bloco->cod));
						if (tk == TKPontoeVirg)
						{
							strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
							leToken();
							if (tk == TKEnd)
							{
								strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
								analise->ret = 1;
								return analise;
							}
							erroEnd();
							analise->ret = 0;
							return analise;
						}
						msgErro("Ponto e virgula esperado");
						analise->ret = 0;
						return analise;
					}
					analise->ret = 0;
					return analise;
				}
				msgErro("Ponto e virgula esperado");
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

_ret *CASEVALUE2()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	_ret *cons = cte();
	if (cons->ret)
	{
		strncat_s(analise->cod, 1000, cons->cod, strlen(cons->cod));
		if (tk == TKVirgula)
		{
			strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			leToken();
			_ret *casevalue2 = CASEVALUE2();
			if (casevalue2->ret)
			{
				strncat_s(analise->cod, 1000, casevalue2->cod, strlen(casevalue2->cod));
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
		strncat_s(analise->cod, 1000, cons->cod, strlen(cons->cod));
		if (tk == TKVirgula)
		{
			strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			leToken();
			_ret *casevalue2 = CASEVALUE2();
			if (casevalue2->ret)
			{
				strncat_s(analise->cod, 1000, casevalue2->cod, strlen(casevalue2->cod));
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
	_ret *cons = cte();
	if (cons->ret)
	{
		strncat_s(analise->cod, 1000, cons->cod, strlen(cons->cod));
		analise->ret = 1;
		return analise;
	}
	else if (tk == TKAbreChave)
	{
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *casevalue1 = CASEVALUE1();
		if (casevalue1->ret)
		{
			strncat_s(analise->cod, 1000, cons->cod, strlen(casevalue1->cod));
			if (tk == TKFechaChave)
			{
				strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
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

_ret *CASE()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKCase)
	{
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *casevalue0 = CASEVALUE0();
		if (casevalue0->ret)
		{
			strncat_s(analise->cod, 1000, casevalue0->cod, strlen(casevalue0->cod));
			_ret *bloco = BLOCO();
			if (bloco->ret)
			{
				strncat_s(analise->cod, 1000, bloco->cod, strlen(bloco->cod));
				if (tk == TKCase || tk == TKOtherwise)
				{
					strncat_s(analise->cod, 1000, "CASE AASAS", strlen("CASE AASAS"));
					_ret *casee = CASE();
					if (casee->ret)
					{
						strncat_s(analise->cod, 1000, bloco->cod, strlen(casee->cod));
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
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *bloco = BLOCO();
		if (bloco->ret)
		{
			strncat_s(analise->cod, 1000, bloco->cod, strlen(bloco->cod));
			analise->ret = 1;
			return analise;
		}
		analise->ret = 0;
		return analise;
	}
	analise->ret = 0;
	return analise;
}

_ret *SWITCH()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKSwitch)
	{
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *ident = id();
		if (ident->ret)
		{
			strncat_s(analise->cod, 1000, ident->cod, strlen(ident->cod));
			_ret *casee = CASE();
			if (casee->ret)
			{
				strncat_s(analise->cod, 1000, casee->cod, strlen(casee->cod));
				if (tk == TKEnd)
				{
					strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
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

_ret *WHILE()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKWhile)
	{
		strncat_s(analise->cod, 1000, "ENQUANTO ", strlen("ENQUANTO "));
		leToken();
		if (tk == TKAbrePar)
		{
			strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
			leToken();
			_ret *comp0 = COMP0();
			if (comp0->ret)
			{
				strncat_s(analise->cod, 1000, comp0->cod, strlen(comp0->cod));
				if (tk == TKFechaPar)
				{
					strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
					leToken();
					_ret *bloco = BLOCO();
					if (bloco->ret)
						strncat_s(analise->cod, 1000, bloco->cod, strlen(bloco->cod));
					{
						if (tk == TKEnd)
						{
							//strncat_s(analise->cod,1000, tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
							strncat_s(analise->cod, 1000, "FIMENQUANTO", strlen("FIMENQUANTO"));
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

_ret *FOR()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	if (tk == TKFor)
	{
		strncat_s(analise->cod, 1000, "PARA ", strlen("PARA "));
		leToken();
		_ret *atrib = ATRIB();
		if (atrib->ret)
		{
			strncat_s(analise->cod, 1000, atrib->cod, strlen(atrib->cod));
			if (tk == TKDoisPontos)
			{
				strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
				leToken();
				_ret *val = VAL();
				if (val->ret)
				{
					strncat_s(analise->cod, 1000, val->cod, strlen(val->cod));
					strncat_s(analise->cod, 1000, "\n\t", strlen("\n\t"));
					if (tk == TKDoisPontos)
					{
						strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
						leToken();
						_ret *val2 = VAL();
						if (!val2->ret)
						{
							erroVal();
							analise->ret = 0;
							return analise;
						} else {
							strncat_s(analise->cod, 1000, val2->cod, strlen(val2->cod));
						}
					}
					_ret *bloco = BLOCO();
					if (bloco->ret)
					{
						strncat_s(analise->cod, 1000, bloco->cod, strlen(bloco->cod));
						if (tk == TKEnd)
						{
							//strncat_s(analise->cod,1000, tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
							strncat_s(analise->cod, 1000, "FIMPARA\n", strlen("FIMPARA\n"));
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

	_ret *func = FUNCTION();
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
	_ret *aid = id();
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

_ret *COMANDO()
{
	_ret *analise = (_ret*)malloc(sizeof(_ret) * 1);
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
	int marcaPos = setPos();

	_ret *atrib = ATRIB();

	if (atrib->ret)
	{
		strncpy_s(analise->cod, 1000, atrib->cod, strlen(atrib->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *fora = FOR(); 
	if (fora->ret)
	{
		strncpy_s(analise->cod, 1000, fora->cod, strlen(fora->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *whileC = WHILE();
	if (whileC->ret)
	{
		strncat_s(analise->cod, 1000, whileC->cod, strlen(whileC->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *switchC = SWITCH();
	if (switchC->ret)
	{
		strncat_s(analise->cod, 1000, switchC->cod, strlen(switchC->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *cmdIf = IF();
	if (cmdIf->ret)
	{
		strncpy_s(analise->cod,1000,cmdIf->cod,strlen(cmdIf->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *tryC = TRY();
	if (tryC->ret)
	{
		strncat_s(analise->cod, 1000, tryC->cod, strlen(tryC->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *parFor = PARFOR();
	if (parFor->ret)
	{
		strncat_s(analise->cod, 1000, parFor->cod, strlen(parFor->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *criaFunction = CRIAFUNCTION();
	if (criaFunction->ret)
	{
		strncat_s(analise->cod, 1000, criaFunction->cod, strlen(criaFunction->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret *function = FUNCTION();
	if (function->ret)
	{
		strncat_s(analise->cod, 1000, function->cod, strlen(function->cod));
		analise->ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	if (tk == TKBreak)
	{
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		analise->ret = 1;
		return analise;
	}
	else if (tk == TKContinue)
	{
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		analise->ret = 1;
		return analise;
	}
	else if (tk == TKReturn)
	{
		strncat_s(analise->cod, 1000, tokens[posTK].elemento, strlen(tokens[posTK].elemento));
		leToken();
		_ret *val = VAL();
		if (val->ret)
		{
			strncat_s(analise->cod, 1000, val->cod, strlen(val->cod));
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

_ret *BLOCO()
{

	_ret *analise_bl = (_ret*)malloc(sizeof(_ret) * 1);
	analise_bl->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise_bl->cod, 1000, "", strlen(""));
	_ret *comando = COMANDO();
	//strncpy_s(comando->cod, "", sizeof(""));
	if (comando->ret)
	{
		strncpy_s(analise_bl->cod, 1000, comando->cod, strlen(comando->cod));
		if (tk == TKPontoeVirg)
		{
			strncat_s(analise_bl->cod, 1000, ";\n", strlen(";\n"));
			leToken();
		}
		if (tk == TKId || tk == TKFor || tk == TKWhile || tk == TKSwitch ||
			tk == TKIf || tk == TKTry || tk == TKParfor || tk == TKFunction ||
			tk == TKBreak || tk == TKContinue || tk == TKReturn)
		{

	//		strncat_s(analise_bl->cod,1000, tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			_ret *bloco = BLOCO();
			if (bloco->ret)
			{
				strncat_s(analise_bl->cod, 1000, bloco->cod, strlen(bloco->cod));
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
	analise->cod = (char *)malloc(sizeof(char) * 1000);
	strncpy_s(analise->cod, 1000, "", strlen(""));
		//necessário para strncat (strncat não concatena strings sem possuir terminador nulo \0
		//ou strncpy_s ou string[strlen(string)] = '\0'; porém a última alternativa resulta em duas strings gigantes contactenadas
	
	_ret *bloco = BLOCO();
	// strncpy_s(bloco->cod, "", sizeof(""));
	//tmp = &bloco;
	//tmp = (_ret *)malloc(sizeof(_ret) * 3000);
	//memset(tmp, 0, sizeof(_ret) * 1);
	if (bloco->ret)
	{
		analise->ret = 1;
		strncat_s(analise->cod, 1000, "algoritmo 'compiladores'\n\n", strlen("algoritmo 'compiladores'\n\n"));
		strncat_s(analise->cod, 1000, "var\n", strlen("var\n"));
		int indice;
		for (indice = 0; indice < nroVariaveis; indice++) {
			if (indice > 0) {
				strncat_s(analise->cod, 1000, ", ", strlen(", "));
			}
			strncat_s(analise->cod, 1000, variaveis[indice], strlen(variaveis[indice]));
		}
		if (nroVariaveis > 0) {
			strncat_s(analise->cod, 1000, " : real\n", strlen(" : real\n"));
		}
		strncat_s(analise->cod, 1000, "\ninicio\n", strlen("inicio\n"));
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
		strncat_s(analise->cod, strlen(analise->cod) + strlen("\nfimalgoritmo\n") +1, 
			"\nfimalgoritmo\n", strlen("\nfimalgoritmo\n"));
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

	portugues = fopen("Portugues.lex", "wb+");

	if (portugues == NULL)
	{
		printf("Erro ao abrir o arquivo de saida (Portugues.lex).\n");
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

	getchar();
	system("pause");
	return 0;
}