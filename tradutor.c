#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256

void traduzirfuncao();
void attr();
void acessArray();
int alinhamento();

char reg1[] = "r8";
char reg2[] = "r9";
char reg3[] = "r10";
char reg4[] = "r11";

typedef struct
{
    char tipo;
    char *registrador;
    int tamanho;
}var;

int main()
{
    char line[MAX];
    char *word;
    printf("# Tradução BPL para Assembly\n\n");
    while(scanf("%[^\n]\n", line) != EOF)
    { //Le linhas
        word = strtok(line," ");          //Quebra a linha em palavras p/ num1entificar 
        while(word!=NULL)
        {
            if(strcmp(word,"function")==0)
            {
                traduzirfuncao(word);     //Inicializa Tradução de uma função
            }
            word = strtok(NULL," ");
        }
    }
    return 0;
}
/*---------------------------------------------------------*/
void traduzirfuncao(char *word)
{
    //Inicializa a pilha da função

    //Nome da Função
    word = strtok(NULL," "); //Pula p/próxima palavra
    printf("\n.globl %s\n\n%s:\n\n",word,word); //printando .globl f1 e f1:
    printf("pushq %%rbp\nmovq %%rsp, %%rbp\n\n");
    word = strtok(NULL," ");

    //Parametros da Função
    var parametros[4]=
    {
        [1].registrador="di",
        [2].registrador="si",
        [3].registrador="dx",
        [1].tamanho=0,
        [2].tamanho=0,
        [3].tamanho=0,
        [1].tipo=0,
        [2].tipo=0,
        [3].tipo=0,
    };

    while(word != NULL)
    { //Salva tipo do parametro na struct (array ou int)
    	// word[2] é igual ao indice do parametro, porém é lido como caracter, por isso é -48, pois "1" na tabela ascii é 49.
        parametros[word[2]-48].tipo = word[1];
        word = strtok(NULL," ");
    }

    //Declara as variaveis locais
    char line[MAX];
    
    var variaveis[13] = 
    {
        [1].registrador="",
        [2].registrador="",
        [3].registrador="",
        [4].registrador="",
        [5].registrador="",
        [6].registrador="",
        [7].registrador="",
        [8].registrador="",
        [9].registrador="",
        [10].registrador="",
        [11].registrador="",
        [12].registrador="",
        [1].tipo=0,
        [2].tipo=0,
        [3].tipo=0,
        [4].tipo=0,
        [5].tipo=0,
        [6].tipo=0,
        [7].tipo=0,
        [8].tipo=0,
        [9].tipo=0,
        [10].tipo=0,
        [11].tipo=0,
        [12].tipo=0,
        [1].tamanho=0,
        [2].tamanho=0,
        [3].tamanho=0,
        [4].tamanho=0,
        [5].tamanho=0,
        [6].tamanho=0,
        [7].tamanho=0,
        [8].tamanho=0,
        [9].tamanho=0,
        [10].tamanho=0,
        [11].tamanho=0,
        [12].tamanho=0,              
    };

    int v;
    int contReg = 1;

    while(scanf("%[^\n]\n", line), strcmp(line,"enddef")!=0)
    {
        sscanf (line, "var vi%d",&v);
        sscanf (line, "vet va%d",&v); 
        sscanf (line, "reg vr%d",&v);

        if (v >= 10)
        {
            variaveis[line[7]-38].tipo = line[5];
            variaveis[line[7]-38].tamanho = 4;

            if(variaveis[line[7]-38].tipo == 'r')
            {
                switch (contReg)
                {
                case 1:
                    variaveis[line[7]-38].registrador = reg1;    
                    contReg++;
                    break;
                case 2:
                    variaveis[line[7]-38].registrador = reg2;    
                    contReg++;
                    break;
                case 3:
                    variaveis[line[7]-38].registrador = reg3;    
                    contReg++;
                    break;
                case 4:
                    variaveis[line[7]-38].registrador = reg4;    
                    contReg++;
                    break;
                }
            }
            // é do tipo = vet vaN size ciN
            if(strlen(line)>9)
            { 
                //leitura número (positivo/negativo)
                int num=0; int sinal = 1; int i=16; 
                if(!(line[i]>='0' && line[i]<='9'))
                {
                    sinal = -1; //negativo
                    i++;
                }
                while(line[i]>='0' && line[i]<='9')
                {
                    num = num*10 + line[i]-'0';
                    i++;
                }
                num*=sinal;
                variaveis[line[7]-38].tamanho *= num;
            }
        }
        else
        {
            variaveis[line[6]-'0'].tipo = line[5];
            variaveis[line[6]-'0'].tamanho = 4;

            if(variaveis[line[6]-'0'].tipo == 'r')
            {
                switch (contReg)
                {
                case 1:
                    variaveis[line[6]-'0'].registrador = reg1;    
                    contReg++;
                    break;
                case 2:
                    variaveis[line[6]-'0'].registrador = reg2;    
                    contReg++;
                    break;
                case 3:
                    variaveis[line[6]-'0'].registrador = reg3;    
                    contReg++;
                    break;
                case 4:
                    variaveis[line[6]-'0'].registrador = reg4;    
                    contReg++;
                    break;
                }
            }
            // é do tipo = vet vaN size ciN
            if(strlen(line)>7)
            { 
                //leitura número (positivo/negativo)
                int num=0; int sinal = 1; int i=15; 
                if(!(line[i]>='0' && line[i]<='9'))
                {
                    sinal = -1; //negativo
                    i++;
                }
                while(line[i]>='0' && line[i]<='9')
                {
                    num = num*10 + line[i]-'0';
                    i++;
                }
                num*=sinal;
                variaveis[line[6]-'0'].tamanho *= num;
            }
        }
    }
   
    int valorpilha;
    //Aumenta Pilha da Função
    int tamanho_pilha = 0;
    //Verifica PARAMETROS Declarados Na função p/alocar na pilha
    int r = 0;

    for (int i = 0; i <= 3; i++)
    {
        if(parametros[i].tipo == 'a')
        {
            parametros[i].tamanho = 8;
        }
        else
        {
            parametros[i].tamanho = 4;
        }
    }

    for(int i=1; i<=3; i++)
    { 
        if(parametros[i].tipo!=0)
        {
            tamanho_pilha+=parametros[i].tamanho; //incrementa tamanho do parametro no tamanho da pilha
            r = alinhamento(parametros[i].tamanho,&tamanho_pilha);
            parametros[i].tamanho=r; //aloca parametro na pilha
            printf("# p%c%d: -%d(%%rbp)\n",  parametros[i].tipo, i, tamanho_pilha ); 
        }
    }

    
    for(int i=1; i<=12; i++)
    {
        if(variaveis[i].tipo!=0)
        { 
            //Verifica VARIAVEIS Declarados Na função p/alocar na pilha
            tamanho_pilha+=variaveis[i].tamanho; 
            variaveis[i].tamanho=tamanho_pilha;  //aloca variaveis na pilha
        
            if (variaveis[i].registrador == reg1 || variaveis[i].registrador == reg2 || variaveis[i].registrador == reg3 || variaveis[i].registrador == reg4)
            {
                printf("# %s: -%d(%%rbp)\n",  variaveis[i].registrador, tamanho_pilha);
            }
            else
            {
                printf("# v%c%d: -%d(%%rbp)\n",  variaveis[i].tipo, i, tamanho_pilha);
            }
        }
    }
    printf ("\n");

    if(tamanho_pilha%16 != 0)
    { 
        tamanho_pilha+=(16-(tamanho_pilha%16)); //Alinha pilha a 16
    }
    if(tamanho_pilha != 0)
    { 
        printf("subq $%d, %%rsp\n\n", tamanho_pilha);
    }
    /*---------------------------------------------------------*/

    //Corpo da Função

    //Atribuição
    int n_if=0;
    while(scanf("%[^\n]\n", line), strcmp(line,"end")!=0)
    {
    
        int xf, num1, num2;
        char l, tipo1, lf1, lf2, ll, tipo2;

        xf = sscanf(line, "if %c%c%d %c%c %c%c%d", &l, &tipo1, &num1, &lf1, &lf2, &ll, &tipo2, &num2); 

        if(xf==8)
        {
            n_if++;
            //  printf("#if %ci%d\n", l, num);
            while(scanf("%[^\n]\n", line), strcmp(line,"endif")!=0)
            {
                //Primeira parte da expressao em %eax
                if(l == 'v')
                {
                    if(tipo1 == 'i')
                    {
                        printf("movl -%d(%%rbp), %%eax\n", variaveis[num1].tamanho);
                    }
                    else
                    {
                        printf("movl %%%sd, %%eax\n", variaveis[num1].registrador);
                    }
                }
                else if(l == 'c')
                {
                    printf("movl $%d, %%eax\n", num1);  
                }
                else if(l == 'p')
                {
                    printf("movl %%e%s, %%eax\n", parametros[num1].registrador);
                }

                //Segunda parte da expressao em %ecx
                if(ll == 'v')
                {
                    if(tipo2 == 'i'){
                        printf("movl -%d(%%rbp), %%ecx\n", variaveis[num2].tamanho);
                    }
                    else{
                        printf("movl %%%sd, %%ecx\n", variaveis[num2].registrador);
                    }        
                }
                else if(ll == 'c')
                {
                    printf("movl $%d, %%ecx\n", num2);  
                }
                else if(ll == 'p')
                { //pi alw
                    printf("movl %%e%s, %%ecx\n", parametros[num2].registrador);
                }
                
                //eq: igual
                if(lf1 == 'e' && lf2 == 'q')
                {   
                    printf("cmpl %%ecx, %%eax\n");
                    printf("jne endif%d\n\n",n_if);
                    attr(line, variaveis, parametros);
                    printf ("\n");
                    printf("endif%d:\n\n",n_if);
                }else

                //ne: não igual
                if(lf1 == 'n' && lf2 == 'e')
                {
                    printf("cmpl %%ecx, %%eax\n");
                    printf("je endif%d\n\n",n_if);
                    attr(line, variaveis, parametros);
                    printf ("\n");
                    printf("endif%d:\n\n",n_if);
                }else

                //lt: menor
                if(lf1 == 'l' && lf2 == 't')
                {
                    printf("cmpl %%ecx, %%eax\n");
                    printf("jge endif%d\n\n",n_if);
                    attr(line, variaveis, parametros);
                    printf ("\n");
                    printf("endif%d:\n\n",n_if);
                }else

                //le: menor igual
                if(lf1 == 'l' && lf2 == 'e')
                {
                    printf("cmpl %%ecx, %%eax\n");
                    printf("jg endif%d\n\n",n_if);
                    attr(line, variaveis, parametros);
                    printf ("\n");
                    printf("endif%d:\n\n",n_if);
                }else

                //gt: maior
                if(lf1 == 'g' && lf2 == 't')
                {
                    printf("cmpl %%ecx, %%eax\n");
                    printf("jle endif%d\n\n",n_if);
                    attr(line, variaveis, parametros);
                    printf ("\n");
                    printf("endif%d:\n\n",n_if);
                }else

                //ge: maior igual
                if(lf1 == 'g' && lf2 == 'e')
                {
                    printf("cmpl %%ecx, %%eax\n");
                    printf("jl endif%d\n\n",n_if);
                    attr(line, variaveis, parametros);
                    printf ("\n");
                    printf("endif%d:\n\n",n_if);
                }

            }
        }
        else
        {
            attr(line, variaveis, parametros);
        }
        
    }
    //end

    printf ("\n");
    printf("leave\nret\n");

    return;
    //Finaliza Função
}

int alinhamento(int align, int *stack)
{
    if (*stack % align == 0 || stack == 0)
    {
        return *stack;
    }
    else
    {
        *stack += (align - (*stack% align));
        
        return *stack;
    }
}

void attr(char * line,  var * variaveis, var * parametros)
{
    int r, s, num1, num2, num3; // para vi
    char op, l, ll, tipo, tipo1, tipo2; // para vi
   
    //    pegando ex: vi2 = ci2 + ci1
    r = sscanf(line, "v%c%d = %c%c%d %c %c%c%d", &tipo, &num1, &l, &tipo1, &num2, &op, &ll, &tipo2, &num3);  //vi
    //printf ("r = %d\n",r);

    int n, f, n1, n2, n3;
    char tipof, l1, l11, l2, l22, l3, l33;

    //    pegando ex: vi2 = call f1 vr1 va3
    s = sscanf(line, "v%c%d = call f%d %c%c%d %c%c%d %c%c%d", &tipof, &n, &f, &l1, &l11, &n1, &l2, &l22, &n2, &l3, &l33, &n3);


    //printf ("s = %d\n",s);
    /*-----------------------------------------------------------------------------------*/
    //<attr> -> <varint> = <valint> (atribuição simples)
    if(r==5)
    {
        if(tipo == 'i')
        {
            if(l == 'c')
            {
                //    printf("#vi%d = ci%d\n", num1, num2);
                printf("movl $%d, -%d(%%rbp)\n\n", num2, variaveis[num1].tamanho);
                //printf ("1\n");
                return;
            }
            if(l == 'v')
            {
                if(tipo1 == 'i'){
                    //   printf("#vi%d = vi%d\n", num1, num2);
                    printf("movl -%d(%%rbp), %%eax\n", variaveis[num2].tamanho);
                    printf("movl %%eax, -%d(%%rbp)\n\n", variaveis[num1].tamanho);
                    //printf ("2\n");
                    return;
                }
                else{
                    //   printf("#vi%d = vi%d\n", num1, num2);
                    printf("movl %%%sd, %%eax\n", variaveis[num2].registrador);
                    printf("movl %%eax, -%d(%%rbp)\n\n", variaveis[num1].tamanho);
                    //printf ("2\n");
                    return;
                }
            }
            if(l == 'p')
            {
                //  printf("#vi%d = pi%d\n", num1, num2);
                printf("movl %%e%s, -%d(%%rbp)\n", parametros[num2].registrador, parametros[num2].tamanho);   
                printf("movl -%d(%%rbp), %%eax\n", parametros[num2].tamanho);
                printf("movl %%eax, -%d(%%rbp)\n\n", variaveis[num1].tamanho);  
                //printf ("3\n");
                return;
            }
        }
        else
        {
            if(l == 'c')
            {
                //    printf("#vi%d = ci%d\n", num1, num2);
                printf("movl $%d, %%%sd\n\n", num2, variaveis[num1].registrador);
                //printf ("1\n");
                return;
            }
            if(l == 'v')
            {
                //   printf("#vi%d = vi%d\n", num1, num2);
                printf("movl -%d(%%rbp), %%eax\n", variaveis[num2].tamanho);
                printf("movl %%eax, %%%sd\n\n", variaveis[num1].registrador);
                //printf ("2\n");
                return;
            }
            if(l == 'p')
            {
                //  printf("#vi%d = pi%d\n", num1, num2);
                printf("movl %%e%s, -%d(%%rbp)\n", parametros[num2].registrador, parametros[num2].tamanho);   
                printf("movl -%d(%%rbp), %%eax\n", parametros[num2].tamanho);
                printf("movl %%eax, %s\n\n", variaveis[num1].registrador);  
                //printf ("3\n");
                return;
            }
        }
    }   
    /*-----------------------------------------------------------------------------------*/
    //<attr> -> <varint> = <oper> (Atribuiçao por expressão)
    if(r==9)
    { 
        //printf("vi%d = %ci%d %c %ci%d\n", num1, l, num2, op, ll, num3); 

        //Primeira parte da expressao em %eax
        if(l == 'v')
        {
            if(tipo1 == 'i')
            {
                printf("movl -%d(%%rbp), %%eax\n", variaveis[num2].tamanho);
            }
            else
            {
                printf("movl %%%sd, %%eax\n", variaveis[num2].registrador);
            }
        }
        else if(l == 'c')
        {
            printf("movl $%d, %%eax\n", num2);  
        }
        else if(l == 'p')
        {
            printf("movl %%e%s, %%eax\n", parametros[num2].registrador);
        }

        //Segunda parte da expressao em %ecx
        if(ll == 'v')
        {
            if(tipo2 == 'i'){
                printf("movl -%d(%%rbp), %%ecx\n", variaveis[num3].tamanho);
            }
            else{
                printf("movl %%%s, %%ecx\n", variaveis[num3].registrador);
            }        
        }
        else if(ll == 'c')
        {
            printf("movl $%d, %%ecx\n", num3);  
        }
        else if(ll == 'p')
        { //pi alw
            printf("movl %%e%s, %%ecx\n", parametros[num3].registrador);
        }

        if(op == '+')
            printf("addl %%ecx, %%eax\n");

        if(op == '-')
            printf("subl %%ecx, %%eax\n");

        if(op == '*')
            printf("imull %%ecx, %%eax\n");

        if(op == '/')
        {      
            printf("cltd\n");
            printf("idivl %%ecx\n");
        } 
          
        //<varint> = <exp>
        if(tipo == 'i'){
            printf("movl %%eax, -%d(%%rbp)\n\n", variaveis[num1].tamanho); 
        }
        else{
            printf("movl %%eax, %%%sd\n\n", variaveis[num1].registrador); 
        }   
        return;
    }

    /*-----------------------------------------------------------------------------------*/

    //<attr> -> <varint> = <call>

    if(s==3)
    { // chamada de função com 0 parâmetros 
      //  printf("#vi%d = f%d\n", n, f);

        for (int i = 0; i <= 3; i++)
        {
            if (parametros[i].tipo == 'i')
            {
                printf ("movl %%e%s, -%d(%%rbp)\n", parametros[i].registrador, parametros[i].tamanho);
            }
            if (parametros[i].tipo == 'a')
            {
                printf ("movq %%r%s, -%d(%%rbp)\n", parametros[i].registrador, parametros[i].tamanho);
            }
        }

        for (int i = 0; i <= 12; i++)
        {
            if (variaveis[i].tipo == 'r')
            {
                printf ("movl %%%sd, -%d(%%rbp)\n",variaveis[i].registrador,variaveis[i].tamanho);
            }
        }

        printf("call f%d\n", f);
        printf("movl %%eax, -%d(%%rbp)\n\n", variaveis[n].tamanho);  

        // recupera os parâmetros
        for (int i = 0; i <= 3; i++)
        {
            if (parametros[i].tipo == 'i')
            {
                printf ("movl -%d(%%rbp), %%e%s\n", parametros[i].tamanho, parametros[i].registrador);
            }
            if (parametros[i].tipo == 'a')
            {
                printf ("movq -%d(%%rbp), %%r%s\n", parametros[i].tamanho, parametros[i].registrador);
            }
        }

        printf ("\n");

        for (int i = 0; i <= 12; i++)
        {
            if (variaveis[i].tipo == 'r')
            {
                printf ("movl -%d(%%rbp), %%%sd\n",variaveis[i].tamanho,variaveis[i].registrador);
            }
        }
        printf ("\n");

        return;
    }

    if(s > 5 && s < 13)
    { // chamada de função com 1 parâmetros a 3 parâmetros
        
        int tmp1, tmp2, tmp3;
        // verifica as variáveis de registradores


        for (int i = 0; i <= 3; i++)
        {
            if (parametros[i].tipo == 'i')
            {
                printf ("movl %%e%s, -%d(%%rbp)\n", parametros[i].registrador, parametros[i].tamanho);
            }
            if (parametros[i].tipo == 'a')
            {
                printf ("movq %%r%s, -%d(%%rbp)\n", parametros[i].registrador, parametros[i].tamanho);
            }
            
        }

        for (int i = 0; i <= 12; i++)
        {
            if (variaveis[i].tipo == 'r')
            {
                printf ("movl %%%sd, -%d(%%rbp)\n",variaveis[i].registrador,variaveis[i].tamanho);
            }
        }

        if(l1 == 'v' || l1 == 'p' || l1 == 'c')
        { 
            //Primeiro parâmetro
            if(l1 =='c')
            {
                printf("movl $%d, %%edi\n", n1); 
            }

            // para parâmetro piN
            if(l11 =='i' && l1 != 'c')
            {
                tmp1 = ( l1 =='p') ? parametros[n1].tamanho : variaveis[n1].tamanho;
                printf("movl -%d(%%rbp), %%edi\n", tmp1);
            }

            if (l11 == 'r')
            {
                printf("movl %%%sd, %%edi\n", variaveis[n1].registrador);
            }
            // para parâmetro paN
            if(l11 == 'a')
            {
                if(l1 == 'p')

                    printf("movq -%d(%%rbp), %%rdi\n", parametros[n1].tamanho);
                
                if(l1 == 'v')
                {
                    printf("leaq -%d(%%rbp), %%rdi\n", variaveis[n1].tamanho);
                }
            }
        }

        if(s==6)
        { //Finaliza a função caso tenha 1 parâmetro

            printf ("\n");            
            printf("call f%d\n", f);
            printf("movl %%eax, -%d(%%rbp)\n\n", variaveis[n].tamanho);

            for (int i = 0; i <= 3; i++)
            {
                if (parametros[i].tipo == 'i')
                {
                    printf ("movl -%d(%%rbp), %%e%s\n", parametros[i].tamanho, parametros[i].registrador);
                }
                if (parametros[i].tipo == 'a')
                {
                    printf ("movq -%d(%%rbp), %%r%s\n", parametros[i].tamanho, parametros[i].registrador);
                }
            }

            for (int i = 0; i <= 12; i++)
            {
                if (variaveis[i].tipo == 'r')
                {
                    printf ("movl -%d(%%rbp), %%%sd\n",variaveis[i].tamanho,variaveis[i].registrador);
                }
            }
            printf ("\n");

            return;
        }

        if(l2=='v' || l2 =='p' || l2 =='c')
        {//Segundo parâmetro
            if(l2== 'c')
            {
                printf("movl $%d, %%esi\n", n2);   
            }
            if(l22=='i' && l2 != 'c')
            {
                tmp2 = ( l2 =='p') ? parametros[n2].tamanho : variaveis[n2].tamanho;
                printf("movl -%d(%%rbp), %%esi\n", tmp2);
            }
            if (l22 == 'r')
            {
                printf("movl %%%sd, %%esi\n", variaveis[n2].registrador);
            }
            if(l22=='a')
            {
                if(l2=='p')
                    printf("movq -%d(%%rbp), %%rsi\n", parametros[n2].tamanho);
                if(l2=='v')
                {
                    printf("leaq -%d(%%rbp), %%rsi\n", variaveis[n2].tamanho);
                }
            }
        }

        if(s==9)
        {//Finaliza a função caso tenha 2 parâmetros
            printf ("\n");
            printf("call f%d\n", f);
            printf("movl %%eax, -%d(%%rbp)\n\n", variaveis[n].tamanho);
            
            for (int i = 0; i <= 3; i++)
            {
                if (parametros[i].tipo == 'i')
                {
                    printf ("movl -%d(%%rbp), %%e%s\n", parametros[i].tamanho, parametros[i].registrador);
                }
                if (parametros[i].tipo == 'a')
                {
                    printf ("movq -%d(%%rbp), %%r%s\n", parametros[i].tamanho, parametros[i].registrador);
                }
                
            } 

            for (int i = 0; i <= 12; i++)
            {
                if (variaveis[i].tipo == 'r')
                {
                    printf ("movl -%d(%%rbp), %%%sd\n",variaveis[i].tamanho,variaveis[i].registrador);
                }
            }
            printf ("\n");
            return;
        }

        if(l3=='v' || l3 =='p' || l3=='c')
        {//Terceiro parâmetro
            if(l3== 'c')
            {
                printf("movl $%d, %%edx\n", n3);   
            }
            if(l33=='i' && l3 != 'c')
            {
                tmp3 = ( l3 =='p') ? parametros[n3].tamanho : variaveis[n3].tamanho;
                printf("movl -%d(%%rbp), %%edx\n", tmp3);
            }
            if (l33 == 'r')
            {
                printf("movl %%%sd, %%edx\n", variaveis[n3].registrador);
            }
            if(l33=='a')
            {
                if(l3=='p')
                printf("movq -%d(%%rbp), %%rdx\n", parametros[n3].tamanho);
                if(l3=='v')
                {
                    printf("leaq -%d(%%rbp), %%rdx\n", variaveis[n3].tamanho);
                }
            }
        }

        if(s==12)
        { //Finaliza a função caso tenha 3 parâmetro
            printf ("\n");
            printf("call f%d\n", f);
            printf("movl %%eax, -%d(%%rbp)\n\n", variaveis[n].tamanho);

            for (int i = 0; i <= 3; i++)
            {
                if (parametros[i].tipo == 'i')
                {
                    printf ("movl -%d(%%rbp), %%e%s\n", parametros[i].tamanho, parametros[i].registrador);
                }
                if (parametros[i].tipo == 'a')
                {
                    printf ("movq -%d(%%rbp), %%r%s\n", parametros[i].tamanho, parametros[i].registrador);
                }
                
            }
            printf ("\n"); 

            for (int i = 0; i <= 12; i++)
            {
                if (variaveis[i].tipo == 'r')
                {
                    printf ("movl -%d(%%rbp), %%%sd\n",variaveis[i].tamanho,variaveis[i].registrador);
                }
            }
            printf ("\n");
            return;
        }  
    }
    /*-----------------------------------------------------------------------------------*/
    char g, arr, ret1,ret2,tipog1,tipog2;
    int arrN, ciN, varN;

    g = sscanf(line, "%cet %ca%d index ci%d to %c%c%d", &g, &arr, &arrN, &ciN, &tipog1 , &tipog2, &varN); 
    
    if(g==7)
    { // <arrayget> → 'get' <array> 'index' <const> 'to' <varint>
        if(arr=='p') 
        {
            if(tipog1 == 'v')
            {
                if(tipog2 == 'i')
                {
                    acessArray(ciN,parametros[arrN]);
                    printf("movl (%%rax), %%ecx\n");
                    printf("movl %%ecx, -%d(%%rbp)\n\n", variaveis[varN].tamanho);
                }
                else if (tipog2 == 'r')
                {
                    acessArray(ciN,parametros[arrN]);
                    printf("movl (%%rax), %%ecx\n");
                    printf("movl %%ecx, -%d(%%rbp)\n\n", variaveis[varN].tamanho);
                }
            }
            else if (tipog1 == 'p')
            {
                if(tipog2 == 'i')
                {
                    acessArray(ciN,parametros[arrN]);
                    printf("movl (%%rax), %%ecx\n");
                    printf("movl %%ecx, -%d(%%rbp)\n\n", parametros[varN].tamanho);
                }
                else if (tipog2 == 'a')
                {
                    acessArray(ciN,parametros[arrN]);
                    printf("movl (%%rax), %%ecx\n");
                    printf("movl %%ecx, -%d(%%rbp)\n\n", parametros[varN].tamanho);
                }
            }
            else if (tipog1 == 'c')
            {
                acessArray(ciN,parametros[arrN]);
                printf("movl (%%rax), %%ecx\n");
                printf("movl $%d ,%%ecx\n\n", varN);
            }
            
        }
        else 
            if(arr=='v') 
            {
                if(tipog1 == 'v')
                {
                    if(tipog2 == 'i')
                    {
                        acessArray(ciN,variaveis[arrN]);
                        printf("movl (%%rax), %%ecx\n");
                        printf("movl %%ecx, -%d(%%rbp)\n\n", variaveis[varN].tamanho);
                    }
                    else if (tipog2 == 'r')
                    {
                        acessArray(ciN,variaveis[arrN]);
                        printf("movl (%%rax), %%ecx\n");
                        printf("movl %%ecx, -%d(%%rbp)\n\n", variaveis[varN].tamanho);
                    }
                }
                else if (tipog1 == 'p')
                {
                    if(tipog2 == 'i')
                    {
                        acessArray(ciN,variaveis[arrN]);
                        printf("movl (%%rax), %%ecx\n");
                        printf("movl %%ecx, -%d(%%rbp)\n\n", parametros[varN].tamanho);
                    }
                    else if (tipog2 == 'a')
                    {
                        acessArray(ciN,variaveis[arrN]);
                        printf("movl (%%rax), %%ecx\n");
                        printf("movl %%ecx, -%d(%%rbp)\n\n", parametros[varN].tamanho);
                    }
                }
                else if (tipog1 == 'c')
                {
                    acessArray(ciN,variaveis[arrN]);
                    printf("movl (%%rax), %%ecx\n");
                    printf("movl $%d ,%%ecx\n\n", varN);
                }
            }
    }
    
    s = sscanf(line, "%cet %ca%d index ci%d with %c%c%d", &g, &arr, &arrN, &ciN, &ret1, &ret2 , &varN);

    if(s == 7)
    { // <arrayset> → 'set' <array> 'index' <const> 'with' <valint>
        if(arr=='p') 
        {
            if(ret1 == 'p')
            {
                if (ret2 == 'i')
                {
                    acessArray(ciN,parametros[arrN]);
                    printf("movl -%d(%%rbp), %%ecx\n", parametros[varN].tamanho);
                }
            }
            else if(ret1 == 'v')
            {
                if (ret2 == 'i')
                {
                    acessArray(ciN,parametros[arrN]);
                    printf("movl -%d(%%rbp), %%ecx\n", variaveis[varN].tamanho);
                }
                else if (ret2 == 'r')
                {
                    acessArray(ciN,parametros[arrN]);
                    printf("movl -%d(%%rbp), %%ecx\n", variaveis[varN].tamanho);
                }
            }
            else if(ret1 == 'c')
            {
                if (ret2 == 'i')
                {
                    acessArray(ciN,parametros[arrN]);
                    printf("movl $%d, %%ecx\n", varN);
                }
            }
        }
        if(arr=='v') 
        {
            if(ret1 == 'p')
            {
                if (ret2 == 'i')
                {
                    acessArray(ciN,variaveis[arrN]);
                    printf("movl -%d(%%rbp), %%ecx\n", parametros[varN].tamanho);
                }
            }
            else if(ret1 == 'v')
            {
                if (ret2 == 'i')
                {
                    acessArray(ciN,variaveis[arrN]);
                    printf("movl -%d(%%rbp), %%ecx\n", variaveis[varN].tamanho);
                }
                else if (ret2 == 'r')
                {
                    acessArray(ciN,variaveis[arrN]);
                    printf("movl -%d(%%rbp), %%ecx\n", variaveis[varN].tamanho);
                }
            }
            else if(ret1 == 'c')
            {
                if (ret2 == 'i')
                {
                    acessArray(ciN,variaveis[arrN]);
                    printf("movl $%d, %%ecx\n", varN);
                }
            }
        }

        printf("movl %%ecx, (%%rax)\n\n");
    }

    /*-----------------------------------------------------------------------------------*/
    int rtn;
    char str;
    rtn = sscanf(line, "%ceturn %ci%d",&str, &ret1, &varN);
    if(rtn==3)
    {
        printf("movl ");
        if(ret1 == 'p')
        {
            printf("-%d(%%rbp), %%eax\n", parametros[varN].tamanho);
        }
        if(ret1 =='v')
        {
            printf("-%d(%%rbp), %%eax\n", variaveis[varN].tamanho);
        }
        if(ret1 =='c')
        {
            printf("$%d, %%eax\n", varN);
        }
    }
}

void acessArray(int index, var array)
{
    printf("movq $%d, %%rax\n", index);
    printf("imulq $4, %%rax\n");
    printf("leaq -%d(%%rbp), %%rcx\n", array.tamanho);
    printf("addq %%rcx, %%rax\n");
}