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
	char cod[300];
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

char variaveis[100][50];
int nroVariaveis = 0;

int buscaTS(char *variavel) {
	int indice;

	for (indice = 0; indice < nroVariaveis; indice++) {
		if (strcmp(variavel, variaveis[indice]) == 0) {
			return 1;
		}
	}
	return 0;
}

void incluirTS(char *variavel) {

	if (buscaTS(variavel) == 0) {
		strncpy_s(variaveis[nroVariaveis], variavel,sizeof(variavel));
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

_ret EXP0();
_ret FUNCTION();
_ret EXP1();
_ret COMP0();
_ret BLOCO();
_ret VAL();

_ret id()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	if (tk == TKId)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		incluirTS(tokens[posTK].elemento);
		leToken();
		analise.ret = 1;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret cte()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	if (tk == TKConstanteInteira)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		analise.ret = 1;
		return analise;
	}
	if (tk == TKConstanteReal)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		analise.ret = 1;
		return analise;
	}
	if (tk == TKString)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		analise.ret = 1;
		return analise;
	}
	if (tk == TKTrue)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		analise.ret = 1;
		return analise;
	}
	if (tk == TKFalse)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		analise.ret = 1;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret ATRIB()
{

	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	_ret ident = id();
	if (ident.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), ident.cod, sizeof(ident.cod));
		if (tk == TKAtrib)
		{
			//strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			strncat_s(analise.cod, sizeof(analise.cod), "<-", sizeof("<-"));
			leToken();
			_ret val = VAL();
			if (val.ret)
			{
				analise.ret = 1;
				strncat_s(analise.cod, sizeof(analise.cod), val.cod, sizeof(val.cod));
				return analise;
			}
			erroVal();
			analise.ret = 0;
			return analise;
		}
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret EXPFIM()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));

	if (tk == TKAbrePar)
	{
		strncpy_s(analise.cod, tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		_ret exp1 = EXP1();
		if (exp1.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), exp1.cod, sizeof(exp1.cod));
			if (tk == TKFechaPar)
			{
				strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
				leToken();
				analise.ret = 1;
				return analise;
			}
			erroFechaPar();
			analise.ret = 0;
			return analise;
		}
		analise.ret = 0;
		return analise;
	}
	int marcaPos = setPos();
	_ret ident = id();
	if (ident.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), ident.cod, sizeof(ident.cod));
		analise.ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret cons = cte();
	if (cons.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), cons.cod, sizeof(cons.cod));
		analise.ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret function = FUNCTION();
	if (function.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), function.cod, sizeof(function.cod));
		analise.ret = 1;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret EXP15()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	if (tk == TKSubtracao)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		_ret expFim = EXPFIM();
		if (expFim.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), expFim.cod, sizeof(expFim.cod));
			analise.ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise.ret = 0;
		return analise;
	}
	else {
		_ret expFim2 = EXPFIM();
		if (expFim2.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), expFim2.cod, sizeof(expFim2.cod));
			analise.ret = 1;
			return analise;
		} else {
			analise.ret = 0;
			return analise;
		}
	}
	analise.ret = 0;
	return analise;
}

_ret EXP14()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	if (tk == TKPotencia)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		_ret exp15 = EXP15();
		if (exp15.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), exp15.cod, sizeof(exp15.cod));
			if (tk == TKPotencia)
			{
				strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
				_ret exp14 = EXP14();
				if (exp14.ret)
				{
					strncat_s(analise.cod, sizeof(analise.cod), exp14.cod, sizeof(exp14.cod));
					analise.ret = 1;
					return analise;
				}
				analise.ret = 0;
				return analise;
			}
			analise.ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret EXP13()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));

	_ret exp15 = EXP15();
	if (exp15.ret)
	{
		strncpy_s(analise.cod,exp15.cod,sizeof(exp15.cod));
		if (tk == TKPotencia)
		{
			strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			_ret exp14 = EXP14();
			if (exp14.ret)
			{
				strncpy_s(analise.cod,exp14.cod,sizeof(exp14.cod));
				analise.ret = 1;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 1;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret EXP12()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));

	if (tk == TKEBinario)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		_ret exp13 = EXP13();
		if (exp13.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), exp13.cod, sizeof(exp13.cod));
			if (tk == TKEBinario)
			{
				strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
				_ret exp12 = EXP12();
				if (exp12.ret)
				{
					strncat_s(analise.cod, sizeof(analise.cod), exp12.cod, sizeof(exp12.cod));
					analise.ret = 1;
					return analise;

				}
				analise.ret = 0;
				return analise;
			}
			analise.ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret EXP11()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	
	_ret exp13 = EXP13();
	if (exp13.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), exp13.cod, sizeof(exp13.cod));
		if (tk == TKEBinario)
		{
			strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			_ret exp12 = EXP12();
			if (exp12.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), exp12.cod, sizeof(exp12.cod));
				analise.ret = 1;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 1;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret EXP10()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	if (tk == TKDivisao)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		_ret exp11 = EXP11();
		if (exp11.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), exp11.cod, sizeof(exp11.cod));
			if (tk == TKDivisao)
			{
				strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
				_ret exp10 = EXP10();
				if (exp10.ret)
				{
					strncat_s(analise.cod, sizeof(analise.cod), exp10.cod, sizeof(exp10.cod));
					analise.ret = 1;
					return analise;
				}
				analise.ret = 0;
				return analise;
			}
			analise.ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret EXP9()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	_ret exp11 = EXP11();
	if (exp11.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), exp11.cod, sizeof(exp11.cod));
		if (tk == TKDivisao)
		{
			strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			_ret exp10 = EXP10();
			if (exp10.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), exp10.cod, sizeof(exp10.cod));
				analise.ret = 1;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 1;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret EXP8()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	if (tk == TKMultiplicacao)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		_ret exp9 = EXP9();
		if (exp9.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), exp9.cod, sizeof(exp9.cod));
			if (tk == TKMultiplicacao)
			{
				strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
				_ret exp8 = EXP8();
				if (exp8.ret)
				{
					strncat_s(analise.cod, sizeof(analise.cod), exp8.cod, sizeof(exp8.cod));
					analise.ret = 1;
					return analise;
				}
				analise.ret = 0;
				return analise;
			}
			analise.ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret EXP7()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	_ret exp9 = EXP9();
	if (exp9.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), exp9.cod, sizeof(exp9.cod));
		if (tk == TKMultiplicacao)
		{
			strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			_ret exp8 = EXP8();
			if (exp8.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), exp8.cod, sizeof(exp8.cod));
				analise.ret = 1;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 1;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret EXP6()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	if (tk == TKOuBinario)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		_ret exp7 = EXP7();
		if (exp7.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), exp7.cod, sizeof(exp7.cod));
			if (tk == TKOuBinario)
			{
				strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
				_ret exp6 = EXP6();
				if (exp6.ret)
				{
					strncat_s(analise.cod, sizeof(analise.cod), exp6.cod, sizeof(exp6.cod));
					analise.ret = 1;
					return analise;
				}
				analise.ret = 0;
				return analise;
			}
			analise.ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret EXP5()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	_ret exp7 = EXP7();
	if (exp7.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), exp7.cod, sizeof(exp7.cod));
		if (tk == TKOuBinario)
		{
			strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			_ret exp6 = EXP6();
			if (exp6.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), exp6.cod, sizeof(exp6.cod));
				analise.ret = 1;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 1;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret EXP4()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	if (tk == TKSubtracao)
	{
		strncpy_s(analise.cod, tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		_ret exp5 = EXP5();
		if (exp5.ret)
		{
			if (tk == TKSubtracao)
			{
			strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			_ret exp4 = EXP4();
				if (exp4.ret)
				{
					analise.ret = 1;
					return analise;
				}
				analise.ret = 0;
				return analise;
			}
			analise.ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret EXP3()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	_ret exp5 = EXP5();
	if (exp5.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), exp5.cod, sizeof(exp5.cod));
		if (tk == TKSubtracao)
		{
			strncat_s(analise.cod, sizeof(analise.cod), "-", sizeof("-"));
			_ret exp4 = EXP4();
			if (exp4.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), exp4.cod, sizeof(exp4.cod));
				analise.ret = 1;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 1;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret EXP2()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	if (tk == TKSoma)
	{
		leToken();
		_ret exp3 = EXP3();
		if (exp3.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), exp3.cod, sizeof(exp3.cod));
			if (tk == TKSoma)
			{
				strncat_s(analise.cod, sizeof(analise.cod), "+", sizeof("+"));
				_ret exp2 = EXP2();
				if (exp2.ret)
				{
					strncat_s(analise.cod, sizeof(analise.cod), exp2.cod, sizeof(exp2.cod));
					analise.ret = 1;
					return analise;
				}
				analise.ret = 0;
				return analise;
			}
			analise.ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret EXP1()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	_ret exp3 = EXP3(); 
	if (exp3.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), exp3.cod, sizeof(exp3.cod));
		if (tk == TKSoma)
		{
			strncat_s(analise.cod, sizeof(analise.cod), "+", sizeof("+"));
			_ret exp2 = EXP2();
			if (exp2.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), exp2.cod, sizeof(exp2.cod));
				analise.ret = 1;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 1;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret COMP5()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	if (tk == TKAbrePar)
	{
		strncpy_s(analise.cod,"(",sizeof("("));
		leToken();
		_ret comp0 = COMP0();
		if (comp0.ret){
			strncat_s(analise.cod, sizeof(analise.cod), comp0.cod, sizeof(comp0.cod));
			if (tk == TKFechaPar)
			{
				strncat_s(analise.cod, sizeof(analise.cod), ")", sizeof(")"));
				leToken();
				analise.ret = 1;
				return analise;
			}
			erroFechaPar();
			analise.ret = 0;
			return analise;
		}
		analise.ret = 0;
		return analise;
	}
	_ret exp1 = EXP1();
	if (exp1.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), exp1.cod, sizeof(exp1.cod));
		if (tk == TKMaior || tk == TKMaiorIgual || tk == TKMenor || tk == TKMenorIgual ||
			tk == TKIgual || tk == TKDiferente)
		{
			if (tk == TKIgual) {
				strncat_s(analise.cod, sizeof(analise.cod), "=", sizeof("="));
			} else {
				strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			}
			leToken();

			_ret exp1_1 = EXP1();
			if (exp1_1.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), exp1_1.cod, sizeof(exp1_1.cod));
				analise.ret = 1;
				return analise;
			}
			erroExpInvalida();
			analise.ret = 0;
			return analise;
		}
		analise.ret = 1;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret COMP4()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	if (tk == TKNegacao)
	{
		strncpy_s(analise.cod,"-",sizeof("-"));
		leToken();
		_ret comp5 = COMP5();
		if (comp5.ret)
		{
			analise.ret = 1;
			strncat_s(analise.cod, sizeof(analise.cod), comp5.cod, sizeof(comp5.cod));
			return analise;
		}
		erroExpInvalida();
		analise.ret = 0;
		return analise;
	} else {
		_ret comp5 = COMP5();
		if (comp5.ret) {
			strncat_s(analise.cod, sizeof(analise.cod), comp5.cod, sizeof(comp5.cod));
			analise.ret = 1;
			return analise;
		}
		else
		{
			analise.ret = 0;
			return analise;
		}
	}
	/*else if (COMP5())
	{
		//strncat_s(analise.cod, sizeof(analise.cod), comp5.cod, sizeof(comp5.cod));
		analise.ret = 1;
		return analise;
	}
	else
	{
		analise.ret = 0;
		return analise;
	}*/
}

_ret COMP3()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	if (tk == TKELogico)
	{
		//strncat_s(analise.cod, sizeof(analise.cod), "&", sizeof("&"));
		//strncat_s(analise.cod, sizeof(analise.cod), " e ", sizeof(" e "));
		leToken();
		_ret comp4 = COMP4();
		if (comp4.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), comp4.cod, strlen(comp4.cod));
			if (tk == TKELogico)
			{
		//		strncat_s(analise.cod, sizeof(analise.cod), "&", sizeof("&"));
				_ret comp3 = COMP3();
				if (comp3.ret)
				{
					strncat_s(analise.cod, sizeof(analise.cod), comp3.cod, sizeof(comp3.cod));
					analise.ret = 1;
					return analise;
				}
				analise.ret = 0;
				return analise;
			}
			analise.ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise.ret = 0;
		return analise;
	}
	else
	{
		analise.ret = 0;
		return analise;		
	}
}

_ret COMP2()
{

	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	_ret comp4 = COMP4();
	if (comp4.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), comp4.cod, sizeof(comp4.cod));
		if (tk == TKELogico)
		{
			//strncat_s(analise.cod, sizeof(analise.cod), "&", sizeof("&"));
			strncat_s(analise.cod, sizeof(analise.cod), " e ", sizeof(" e "));
			_ret comp3 = COMP3();
			if (comp3.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), comp3.cod, sizeof(comp3.cod));
				analise.ret = 1;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 1;
		return analise;
	}
	else
	{
		analise.ret = 0;
		return analise;
	}
}

_ret COMP1()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	if (tk == TKOuLogico)
	{
		//strncpy_s(analise.cod,"|",sizeof("|"));
		//strncpy_s(analise.cod," ou ",sizeof(" ou "));
		leToken();
		_ret comp2 = COMP2();
		if (comp2.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), comp2.cod, sizeof(comp2.cod));
			if (tk == TKOuLogico)
			{
				//strncat_s(analise.cod, sizeof(analise.cod), "|", sizeof("|"));
				_ret comp1 = COMP1();
				if (comp1.ret)
				{
					strncat_s(analise.cod, sizeof(analise.cod), comp1.cod, sizeof(comp1.cod));
					analise.ret = 1;
					return analise;
				}
				analise.ret = 0;
				return analise;
			}
			analise.ret = 1;
			return analise;
		}
		erroExpInvalida();
		analise.ret = 0;
		return analise;

	}
	else
	{
		analise.ret = 0;
		return analise;
	}
}

_ret COMP0()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	_ret comp2 = COMP2();
	if (comp2.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), comp2.cod, sizeof(comp2.cod));
		if (tk == TKOuLogico)
		{
			strncat_s(analise.cod, sizeof(analise.cod), " ou ", sizeof(" ou "));
			_ret comp1 = COMP1();
			if (comp1.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), comp1.cod, sizeof(comp1.cod));
				analise.ret = 1;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 1;
		return analise;
	}
	else
	{
		analise.ret = 0;
		return analise;
	}
}

_ret ELSE()
{
	_ret analise;
	strncpy_s(analise.cod, "", sizeof(""));
	if (tk == TKElse)
	{
		strncpy_s(analise.cod, "SENAO\n\t", sizeof("SENAO\n\t"));
		leToken();
		_ret bloco = BLOCO();
		if (bloco.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), bloco.cod, sizeof(bloco.cod));
			analise.ret = 1;
			return analise;
		}
	}
	else if (tk == TKElseIf)
	{
		strncat_s(analise.cod, sizeof(analise.cod), "SENAO SE\n\t", sizeof("SENAO SE\n\t"));
		leToken();
		if (tk == TKAbrePar)
		{
			strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			leToken();
			_ret comp0 = COMP0();
			if (comp0.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), comp0.cod, sizeof(comp0.cod));
				if (tk == TKFechaPar)
				{
					strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
					leToken();
					_ret bloco = BLOCO();
					if (bloco.ret)
					{
						strncat_s(analise.cod, sizeof(analise.cod), bloco.cod, sizeof(bloco.cod));
						if (tk == TKElse || tk == TKElseIf)
						{
							strncat_s(analise.cod, sizeof(analise.cod), "SENAO SE VER", sizeof("SENAO SE VER"));
							_ret elsee = ELSE();
							if (elsee.ret)
							{
								strncat_s(analise.cod, sizeof(analise.cod), elsee.cod, sizeof(elsee.cod));
								analise.ret = 1;
								return analise;
							}
							analise.ret = 0;
							return analise;
						}
						analise.ret = 1;
						return analise;
					}
					analise.ret = 0;
					return analise;
				}
				erroFechaPar();
				analise.ret = 0;
				return analise;
			}
			erroAbrePar();
			analise.ret = 0;
			return analise;
		}
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret IF()
{
	_ret analise;
	strncpy_s(analise.cod,"", sizeof(""));
	if (tk == TKIf)
	{
		strncpy_s(analise.cod,"\nSE ", sizeof("\nSE "));
		leToken();
		if (tk == TKAbrePar)
		{
			strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			leToken();
			_ret comp0 = COMP0();
			if (comp0.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), comp0.cod, sizeof(comp0.cod));
				if (tk == TKFechaPar)
				{
					strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
					strncat_s(analise.cod, sizeof(analise.cod), " ENTAO\n\t", sizeof(" ENTAO\n\t"));
					leToken();
					_ret bloco = BLOCO();
					if (bloco.ret)
					{
						strncat_s(analise.cod, sizeof(analise.cod), bloco.cod, sizeof(bloco.cod));
						if (tk == TKElse || tk == TKElseIf)
						{
							/*
							if (tk == TKElseIf) {
								strncat_s(analise.cod, sizeof(analise.cod), "SENAO SE", sizeof("SENAO SE"));
							} else {
								strncat_s(analise.cod, sizeof(analise.cod), "SENAO", sizeof("SENAO"));
							}
							*/

							_ret elsee = ELSE();
							if (!elsee.ret)
							{
								analise.ret = 0;
								return analise;
							} else {
								strncat_s(analise.cod, sizeof(analise.cod), elsee.cod, sizeof(elsee.cod));
							}
						}
						if (tk == TKEnd)
						{
							strncat_s(analise.cod, sizeof(analise.cod), "FIMSE\n", sizeof("FIMSE\n"));
							leToken();
							analise.ret = 1;
							return analise;
						}
						erroEnd();
						analise.ret = 0;
						return analise;
					}
					analise.ret = 0;
					return analise;
				}
				erroFechaPar();
				analise.ret = 0;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		erroAbrePar();
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret TRY()
{	
	_ret analise;
	strncpy_s(analise.cod, "", sizeof(""));
	if (tk == TKTry)
	{
		strncpy_s(analise.cod, "TESTAR ", sizeof("TESTAR"));
		leToken();
		_ret bloco = BLOCO();
		if (bloco.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), bloco.cod, sizeof(bloco.cod));
			if (tk == TKCatch)
			{
				strncat_s(analise.cod, sizeof(analise.cod), "CATCH E", sizeof("CATCH E"));
				leToken();
				_ret bloco2 = BLOCO();
				if (bloco2.ret)
				{
					strncat_s(analise.cod, sizeof(analise.cod), bloco2.cod, sizeof(bloco2.cod));
					if (tk == TKEnd)
					{
						strncat_s(analise.cod, sizeof(analise.cod), "FIM CATCH", sizeof("FIM CATCH"));
						leToken();
						analise.ret = 1;
						return analise;
					}
					erroEnd();
					analise.ret = 0;
					return analise;
				}
				analise.ret = 0;
				return analise;
			}
			msgErro("CATCH faltando!");
			analise.ret = 0;
			return analise;
		}
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret CRIAFUNCTION()
{
	_ret analise;
	strncpy_s(analise.cod, "", sizeof(""));
	if (tk == TKFunction)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		_ret function = FUNCTION();
		if (function.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), function.cod, sizeof(function.cod));
			_ret bloco = BLOCO();
			if (bloco.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), bloco.cod, sizeof(bloco.cod));
				if (tk == TKEnd)
				{
					strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
					leToken();
					analise.ret = 1;
					return analise;
				}
				erroEnd();
				analise.ret = 0;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret PARAM2()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	_ret val = VAL();
	if (val.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), val.cod, sizeof(val.cod));
		analise.ret = 1;
		return analise;
	}
	erroVal();
	analise.ret = 0;
	return analise;
}

_ret PARAM1()
{
	_ret analise;
	strncpy_s(analise.cod, "", sizeof(""));
	if (tk == TKVirgula)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		_ret param2 = PARAM2();
		if (param2.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), param2.cod, sizeof(param2.cod));
			if (tk == TKVirgula)
			{
				strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
				_ret param1 = PARAM1();
				if (param1.ret)
				{
					analise.ret = 1;
					return analise;
				}
				analise.ret = 0;
				return analise;
			}
			analise.ret = 1;
			return analise;
		}
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret PARAM0()
{
	_ret analise;
	strncpy_s(analise.cod, "", sizeof(""));
	_ret param2 = PARAM2();
	if (param2.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), param2.cod, sizeof(param2.cod));
		if (tk == TKVirgula)
		{
			strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			_ret param1 = PARAM1();
			if (param1.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), param1.cod, sizeof(param1.cod));	
				analise.ret = 1;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 1;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret FUNCTION()
{
	_ret analise;
	strncpy_s(analise.cod, "", sizeof(""));

	_ret ident = id();
	if (ident.ret)
	{
		strncpy_s(analise.cod, ident.cod, sizeof(ident.cod));
		if (tk == TKAbrePar)
		{
			strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			leToken();
			_ret param0 = PARAM0();
			if (param0.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), param0.cod, sizeof(param0.cod));
				if (tk == TKFechaPar)
				{
					strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
					leToken();
					analise.ret = 1;
					return analise;
				}
				erroFechaPar();
				analise.ret = 0;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret PARFOR()
{
	_ret analise;
	strncpy_s(analise.cod, "", sizeof(""));
	_ret atrib = ATRIB();
	if (atrib.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), atrib.cod, sizeof(atrib.cod));
		if (tk == TKDoisPontos)
		{
			strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			leToken();
			_ret val = VAL();
			if (val.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), val.cod, sizeof(val.cod));
				if (tk == TKPontoeVirg)
				{
					strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
					leToken();
					_ret bloco = BLOCO();
					if (bloco.ret)
					{
						strncat_s(analise.cod, sizeof(analise.cod), bloco.cod, sizeof(bloco.cod));
						if (tk == TKPontoeVirg)
						{
							strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
							leToken();
							if (tk == TKEnd)
							{
								strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
								analise.ret = 1;
								return analise;
							}
							erroEnd();
							analise.ret = 0;
							return analise;
						}
						msgErro("Ponto e virgula esperado");
						analise.ret = 0;
						return analise;
					}
					analise.ret = 0;
					return analise;
				}
				msgErro("Ponto e virgula esperado");
				analise.ret = 0;
				return analise;
			}
			erroVal();
			analise.ret = 0;
			return analise;
		}
		erroDoisPt();
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret CASEVALUE2()
{
	_ret analise;
	strncpy_s(analise.cod, "", sizeof(""));
	_ret cons = cte();
	if (cons.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), cons.cod, sizeof(cons.cod));
		if (tk == TKVirgula)
		{
			strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			leToken();
			_ret casevalue2 = CASEVALUE2();
			if (casevalue2.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), casevalue2.cod, sizeof(casevalue2.cod));
				analise.ret = 1;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 1;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret CASEVALUE1()
{
	_ret analise;
	strncpy_s(analise.cod, "", sizeof(""));
	_ret cons = cte();
	if (cons.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), cons.cod, sizeof(cons.cod));
		if (tk == TKVirgula)
		{
			strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			leToken();
			_ret casevalue2 = CASEVALUE2();
			if (casevalue2.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), casevalue2.cod, sizeof(casevalue2.cod));
				analise.ret = 1;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret CASEVALUE0()
{
	_ret analise;
	strncpy_s(analise.cod, "", sizeof(""));
	_ret cons = cte();
	if (cons.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), cons.cod, sizeof(cons.cod));
		analise.ret = 1;
		return analise;
	}
	else if (tk == TKAbreChave)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		_ret casevalue1 = CASEVALUE1();
		if (casevalue1.ret)
		{
			strncat_s(analise.cod, sizeof(casevalue1.cod), cons.cod, sizeof(casevalue1.cod));
			if (tk == TKFechaChave)
			{
				strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
				leToken();
				analise.ret = 1;
				return analise;
			}
			msgErro("Falta fechar as chaves");
			analise.ret = 0;
			return analise;
		}
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret CASE()
{
	_ret analise;
	strncpy_s(analise.cod, "", sizeof(""));
	if (tk == TKCase)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		_ret casevalue0 = CASEVALUE0();
		if (casevalue0.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), casevalue0.cod, sizeof(casevalue0.cod));
			_ret bloco = BLOCO();
			if (bloco.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), bloco.cod, sizeof(bloco.cod));
				if (tk == TKCase || tk == TKOtherwise)
				{
					strncat_s(analise.cod, sizeof(analise.cod), "CASE AASAS", sizeof("CASE AASAS"));
					_ret casee = CASE();
					if (casee.ret)
					{
						strncat_s(analise.cod, sizeof(casee.cod), bloco.cod, sizeof(casee.cod));
						analise.ret = 1;
						return analise;
					}
					analise.ret = 0;
					return analise;
				}
				analise.ret = 1;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 0;
		return analise;
	}
	else if (tk == TKOtherwise)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		_ret bloco = BLOCO();
		if (bloco.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), bloco.cod, sizeof(bloco.cod));
			analise.ret = 1;
			return analise;
		}
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret SWITCH()
{
	_ret analise;
	strncpy_s(analise.cod, "", sizeof(""));
	if (tk == TKSwitch)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		_ret ident = id();
		if (ident.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), ident.cod, sizeof(ident.cod));
			_ret casee = CASE();
			if (casee.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), casee.cod, sizeof(casee.cod));
				if (tk == TKEnd)
				{
					strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
					leToken();
					analise.ret = 1;
					return analise;
				}
				erroEnd();
				analise.ret = 0;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret WHILE()
{
	_ret analise;
	strncpy_s(analise.cod, "", sizeof(""));
	if (tk == TKWhile)
	{
		strncat_s(analise.cod, sizeof(analise.cod), "ENQUANTO", sizeof("ENQUANTO"));
		leToken();
		if (tk == TKAbrePar)
		{
			strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			leToken();
			_ret comp0 = COMP0();
			if (comp0.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), comp0.cod, sizeof(comp0.cod));
				if (tk == TKFechaPar)
				{
					strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
					leToken();
					_ret bloco = BLOCO();
					if (bloco.ret)
						strncat_s(analise.cod, sizeof(analise.cod), bloco.cod, sizeof(bloco.cod));
					{
						if (tk == TKEnd)
						{
							strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
							leToken();
							analise.ret = 1;
							return analise;
						}
						erroEnd();
						analise.ret = 0;
						return analise;
					}
					analise.ret = 0;
					return analise;
				}
				erroFechaPar();
				analise.ret = 0;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		erroAbrePar();
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret FOR()
{
	_ret analise;
	strncpy_s(analise.cod, "", sizeof(""));
	if (tk == TKFor)
	{
		strncat_s(analise.cod, sizeof(analise.cod), "PARA ", sizeof("PARA "));
		leToken();
		_ret atrib = ATRIB();
		if (atrib.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), atrib.cod, sizeof(atrib.cod));
			if (tk == TKDoisPontos)
			{
				strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
				leToken();
				_ret val = VAL();
				if (val.ret)
				{
					strncat_s(analise.cod, sizeof(analise.cod), val.cod, sizeof(val.cod));
					strncat_s(analise.cod, sizeof(analise.cod), "\n\t", sizeof("\n\t"));
					if (tk == TKDoisPontos)
					{
						strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
						leToken();
						_ret val2 = VAL();
						if (!val2.ret)
						{
							erroVal();
							analise.ret = 0;
							return analise;
						} else {
							strncat_s(analise.cod, sizeof(analise.cod), val2.cod, sizeof(val2.cod));
						}
					}
					_ret bloco = BLOCO();
					if (bloco.ret)
					{
						strncat_s(analise.cod, sizeof(analise.cod), bloco.cod, sizeof(bloco.cod));
						if (tk == TKEnd)
						{
							strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
							leToken();
							analise.ret = 1;
							return analise;
						}
						erroEnd();
						analise.ret = 0;
						return analise;
					}
					analise.ret = 0;
					return analise;
				}
				erroVal();
				analise.ret = 0;
				return analise;
			}
			erroDoisPt();
			analise.ret = 0;
			return analise;
		}
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret VAL()
{
	int marcaPos = setPos();
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));

	_ret func = FUNCTION();
	if (func.ret)
	{
		strncpy_s(analise.cod, func.cod,sizeof(func.cod));
		analise.ret = 1;
		return analise;
	}
	voltaPos(marcaPos);

	_ret comp0 = COMP0();
	if (comp0.ret)
	{
		strncpy_s(analise.cod,comp0.cod,sizeof(comp0.cod));
		analise.ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret aid = id();
	if (aid.ret)
	{
		strncpy_s(analise.cod, aid.cod,sizeof(aid.cod));
		analise.ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret constan = cte();
	if (constan.ret)
	{
		strncpy_s(analise.cod, constan.cod,sizeof(constan.cod));
		analise.ret = 1;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret COMANDO()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	int marcaPos = setPos();

	_ret atrib = ATRIB();

	if (atrib.ret)
	{
		strncpy_s(analise.cod, atrib.cod,sizeof(atrib.cod));
		analise.ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret fora = FOR(); 
	if (fora.ret)
	{
		strncpy_s(analise.cod, fora.cod,sizeof(fora.cod));
		analise.ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret whileC = WHILE();
	if (whileC.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), whileC.cod, sizeof(whileC.cod));
		analise.ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret switchC = SWITCH();
	if (switchC.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), switchC.cod, sizeof(switchC.cod));
		analise.ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret cmdIf = IF();
	if (cmdIf.ret)
	{
		strncpy_s(analise.cod,cmdIf.cod,sizeof(cmdIf.cod));
		analise.ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret tryC = TRY();
	if (tryC.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tryC.cod, sizeof(tryC.cod));
		analise.ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret parFor = PARFOR();
	if (parFor.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), parFor.cod, sizeof(parFor.cod));
		analise.ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret criaFunction = CRIAFUNCTION();
	if (criaFunction.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), criaFunction.cod, sizeof(criaFunction.cod));
		analise.ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	_ret function = FUNCTION();
	if (function.ret)
	{
		strncat_s(analise.cod, sizeof(analise.cod), function.cod, sizeof(function.cod));
		analise.ret = 1;
		return analise;
	}
	voltaPos(marcaPos);
	if (tk == TKBreak)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		analise.ret = 1;
		return analise;
	}
	else if (tk == TKContinue)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		analise.ret = 1;
		return analise;
	}
	else if (tk == TKReturn)
	{
		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
		leToken();
		_ret val = VAL();
		if (val.ret)
		{
			strncat_s(analise.cod, sizeof(analise.cod), val.cod, sizeof(val.cod));
			analise.ret = 1;
			return analise;
		}
		erroVal();
		analise.ret = 0;
		return analise;
	}
	analise.ret = 0;
	return analise;
}

_ret BLOCO()
{

	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));
	_ret comando = COMANDO();
	if (comando.ret)
	{
		strncpy_s(analise.cod,comando.cod,sizeof(comando.cod));
		if (tk == TKPontoeVirg)
		{
			strncat_s(analise.cod, sizeof(analise.cod), ";\n", sizeof(";\n"));
			leToken();
		}
		if (tk == TKId || tk == TKFor || tk == TKWhile || tk == TKSwitch ||
			tk == TKIf || tk == TKTry || tk == TKParfor || tk == TKFunction ||
			tk == TKBreak || tk == TKContinue || tk == TKReturn)
		{

	//		strncat_s(analise.cod, sizeof(analise.cod), tokens[posTK].elemento, sizeof(tokens[posTK].elemento));
			_ret bloco = BLOCO();
			if (bloco.ret)
			{
				strncat_s(analise.cod, sizeof(analise.cod), bloco.cod, sizeof(bloco.cod));
				analise.ret = 1;
				return analise;
			}
			analise.ret = 0;
			return analise;
		}
		analise.ret = 1;
		return analise;
	}
	else
	{
		analise.ret = 0;
		return analise;
	}
}

/*void atribuirCod(_ret *vet, char string) {
	strncpy_s((*vet).cod, string, sizeof(string));
//	strncpy_s((*vet).cod, "teste", sizeof("teste"));
}*/

_ret INICIO()
{
	_ret analise;
	strncpy_s(analise.cod,"",sizeof(""));

	_ret bloco = BLOCO();
	if (bloco.ret)
	{
		analise.ret = 1;
		strncat_s(analise.cod, sizeof(analise.cod), "algoritmo 'compiladores'\n\n", sizeof("algoritmo 'compiladores'\n\n"));
		strncat_s(analise.cod, sizeof(analise.cod), "var\n", sizeof("var\n"));
		int indice;
		for (indice = 0; indice < nroVariaveis; indice++) {
			if (indice > 0) {
				strncat_s(analise.cod, sizeof(analise.cod), ", ", sizeof(", "));
			}
			strncat_s(analise.cod, sizeof(analise.cod), variaveis[indice], sizeof(variaveis[indice]));
		}
		if (nroVariaveis > 0) {
			strncat_s(analise.cod, sizeof(analise.cod), " : real\n", sizeof(" : real\n"));
		}
		strncat_s(analise.cod, sizeof(analise.cod), "\ninicio\n", sizeof("inicio\n"));
		strncat_s(analise.cod, sizeof(analise.cod), bloco.cod, sizeof(bloco.cod));
		strncat_s(analise.cod, sizeof(analise.cod), "\nfimalgoritmo", sizeof("\nfimalgoritmo"));
		fprintf(portugues, "%s", analise.cod);
		return analise;
	}
	else
	{
		analise.ret = 0;
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

		_ret inicio = INICIO();

		if (!inicio.ret)
		{
			ocorreuErro();
			getchar();
			return 0;
		}
	//	fprintf(portugues, "%s \n", inicio.cod);
		leToken();
	}


	fclose(newFile);
	fclose(portugues);


	printf("Arquivo gerado com sucesso!");

	getchar();
	system("pause");
	return 0;
}