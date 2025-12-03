#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define stricmp _stricmp
#else
#include <strings.h>
#define stricmp strcasecmp
#endif

#define MAX 100
#define pagina 10
typedef struct
{
    char nome[50];
    char classe[20];
    char poder[100];
    char era[20];
    char alinhamento[20];
} Personagem;

/* --- Utilidades --- */
void menu();
int lerString(char *str, int tamanho);
int compara_nomes(const void *a, const void *b);

/* --- CRUD --- */
void adicionar(Personagem vet[], int *qtd);
void listar(Personagem vet[], int qtd);
void listarOrdenado(Personagem vet[], int qtd);
void buscar(Personagem vet[], int qtd);
void editar(Personagem vet[], int qtd);
void excluir(Personagem vet[], int *qtd);
int existenome(Personagem vet[], int qtd, const char *nome);

/* --- Filtros e pesquisas específicas --- */
void filtrarClasse(Personagem vet[], int qtd);
void filtrarEra(Personagem vet[], int qtd);
void filtrarAlinhamento(Personagem vet[], int qtd);

/* --- Submenus --- */
void menuListar(Personagem vet[], int qtd);
void menuFiltrar(Personagem vet[], int qtd);

/* --- Arquivos --- */
void salvar(Personagem vet[], int qtd);
void carregar(Personagem vet[], int *qtd);

int main()
{
    Personagem catalogo[MAX];
    int qtd = 0;
    int opcao;

    carregar(catalogo, &qtd); // carregar ao iniciar

    do
    {
        menu();
        printf("\nEscolha: ");
        if (scanf("%d", &opcao) != 1)
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;
            opcao = -1;
        }
        else
        {
            getchar();
        }

        switch (opcao)
        {
        case 1:
            adicionar(catalogo, &qtd);
            break;
        case 2:
            menuListar(catalogo, qtd);
            break;
        case 3:
            buscar(catalogo, qtd);
            break;
        case 4:
            menuFiltrar(catalogo, qtd);
            break;
        case 5:
            excluir(catalogo, &qtd);
            break;
        case 0:
            salvar(catalogo, qtd); // salvar ao sair
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}

void menu()
{
    printf("\n========== CATALOGO DE PERSONAGENS ==========\n");
    printf("1 - Adicionar personagem\n");
    printf("2 - Listar\n");
    printf("3 - Buscar por nome\n");
    printf("4 - Filtrar\n");
    printf("5 - Excluir personagem\n");
    printf("0 - Sair\n");
}

void menuListar(Personagem vet[], int qtd)
{
    int opcao;

    printf("\n--- Menu de Listagem ---\n");
    printf("1 - Listar todos os personagens\n");
    printf("2 - Listar personagens em ordem alfabetica\n");
    printf("0 - Voltar ao menu principal\n");
    printf("Escolha uma opcao: ");
    if (scanf("%d", &opcao) != 1)
    {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;
        opcao = -1;
    }
    else
    {
        getchar();
    }
    switch (opcao)
    {
    case 1:
        listar(vet, qtd);
        break;
    case 2:
        listarOrdenado(vet, qtd);
        break;
    case 0:
        return;
    default:
        printf("Opcao invalida!\n");
        break;
    }
}
void menuFiltrar(Personagem vet[], int qtd)
{
    int opcao;

    printf("\n--- Menu de Filtro ---\n");
    printf("1 - Filtrar por classe\n");
    printf("2 - Filtrar por era\n");
    printf("3 - Filtrar por alinhamento\n");
    printf("0 - Voltar ao menu principal\n");
    printf("Escolha uma opcao: ");
    if (scanf("%d", &opcao) != 1)
    {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;
        opcao = -1;
    }
    else
    {
        getchar();
    }
    switch (opcao)
    {
    case 1:
        filtrarClasse(vet, qtd);
        break;
    case 2:
        filtrarEra(vet, qtd);
        break;
    case 3:
        filtrarAlinhamento(vet, qtd);
        break;
    case 0:
        return;
    default:
        printf("Opcao invalida!\n");
        break;
    }
}

int lerString(char *str, int tamanho)
{
    if (fgets(str, tamanho, stdin) == NULL)
    {
        str[0] = '\0';
        return 0;
    }
    str[strcspn(str, "\n")] = '\0';
    return (int)strlen(str);
}

void adicionar(Personagem vet[], int *qtd)
{
    if (*qtd >= MAX)
    {
        printf("Catalogo cheio!\n");
        return;
    }

    printf("\n--- Adicionar Personagem ---\n");

    printf("Nome: ");
    lerString(vet[*qtd].nome, sizeof(vet[*qtd].nome));

    // Verifica duplicado
    if (existeNome(vet, *qtd, vet[*qtd].nome))
    {
        printf("Já existe um personagem com esse nome!\n");
        return;
    }

    printf("Classe: ");
    lerString(vet[*qtd].classe, sizeof(vet[*qtd].classe));

    printf("Poder principal: ");
    lerString(vet[*qtd].poder, sizeof(vet[*qtd].poder));

    printf("Era: ");
    lerString(vet[*qtd].era, sizeof(vet[*qtd].era));

    printf("Alinhamento (Heroi, Vilao, Anti-heroi...): ");
    lerString(vet[*qtd].alinhamento, sizeof(vet[*qtd].alinhamento));

    (*qtd)++;
    printf("Personagem adicionado com sucesso!\n");
}

void listar(Personagem vet[], int qtd)
{
    if (qtd == 0)
    {
        printf("Nenhum personagem cadastrado.\n");
        return;
    }
    int p_atual = 0;
    int total_paginas = (qtd + pagina - 1) / pagina;

    while (1)
    {
        int inicio = p_atual * pagina;
        int fim = inicio + pagina;
        if (fim > qtd)
        {
            fim = qtd;
        }
        printf("--Pagina %d/%d (itens %d a %d de %d)--\n", p_atual + 1, total_paginas, inicio + 1, fim, qtd);
        for (int i = inicio; i < fim; i++)
        {
            printf("\n[%d]\n", i + 1);
            printf("Nome: %s\n", vet[i].nome);
            printf("Classe: %s\n", vet[i].classe);
            printf("Poder: %s\n", vet[i].poder);
            printf("Era: %s\n", vet[i].era);
            printf("Alinhamento: %s\n", vet[i].alinhamento);
        }
        printf("\n[ENTER=Proxima] [P=Anterior] [N=Proxima] [0=Sair]: ");

        char buf[16];
        if (fgets(buf, sizeof(buf), stdin) == NULL)
            break;
        char opc = buf[0];

        if (opc == '0')
        {
            break;
        }
        else if (opc == 'p' || opc == 'P')
        {
            if (p_atual > 0)
                p_atual--;
        }
        else /* ENTER, N, n ou qualquer outra tecla -> próxima página ou sair se fim */
        {
            if (p_atual < total_paginas - 1)
                p_atual++;
            else
                break;
        }

#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
    printf("\nFim da listagem.\n");
}

void buscar(Personagem vet[], int qtd)
{
    char nomeBusca[50];
    printf("\nDigite o nome para buscar: ");
    lerString(nomeBusca, sizeof(nomeBusca));

    for (int i = 0; i < qtd; i++)
    {
        if (stricmp(vet[i].nome, nomeBusca) == 0)
        {
            printf("\nPersonagem encontrado!\n");
            printf("Nome: %s\n", vet[i].nome);
            printf("Classe: %s\n", vet[i].classe);
            printf("Poder: %s\n", vet[i].poder);
            printf("Era: %s\n", vet[i].era);
            printf("Alinhamento: %s\n", vet[i].alinhamento);
            return;
        }
    }

    printf("Nenhum personagem com esse nome foi encontrado.\n");
}

void filtrarClasse(Personagem vet[], int qtd)
{
    char classeBusca[20];
    printf("\nDigite a classe: ");
    lerString(classeBusca, sizeof(classeBusca));

    int encontrados = 0;

    printf("\n--- Personagens da Classe %s ---\n", classeBusca);

    for (int i = 0; i < qtd; i++)
    {
        if (stricmp(vet[i].classe, classeBusca) == 0)
        {
            printf("\nNome: %s\n", vet[i].nome);
            printf("Poder: %s\n", vet[i].poder);
            printf("Era: %s\n", vet[i].era);
            printf("Alinhamento: %s\n", vet[i].alinhamento);
            encontrados++;
        }
    }

    if (encontrados == 0)
        printf("Nenhum personagem dessa classe.\n");
}

void filtrarEra(Personagem vet[], int qtd)
{
    char eraBusca[20];
    printf("\nDigite a Era a filtrar: ");
    lerString(eraBusca, sizeof(eraBusca));

    int encontrados = 0;

    printf("\n--- Personagens da Era %s ---\n", eraBusca);

    for (int i = 0; i < qtd; i++)
    {
        if (stricmp(vet[i].era, eraBusca) == 0)
        {
            printf("\nNome: %s\n", vet[i].nome);
            printf("Classe: %s\n", vet[i].classe);
            printf("Poder: %s\n", vet[i].poder);
            printf("Alinhamento: %s\n", vet[i].alinhamento);
            encontrados++;
        }
    }

    if (encontrados == 0)
        printf("Nenhum personagem encontrado nessa era.\n");
}

void filtrarAlinhamento(Personagem vet[], int qtd)
{
    char alinhamentoBusca[20];
    printf("\nDigite o Alinhamento a filtrar: ");
    lerString(alinhamentoBusca, sizeof(alinhamentoBusca));
    int encontrados = 0;
    printf("\n--- Personagens com Alinhamento %s ---\n", alinhamentoBusca);
    for (int i = 0; i < qtd; i++)
    {
        if (stricmp(vet[i].alinhamento, alinhamentoBusca) == 0)
        {
            printf("\nNome: %s\n", vet[i].nome);
            printf("Classe: %s\n", vet[i].classe);
            printf("Poder: %s\n", vet[i].poder);
            printf("Era: %s\n", vet[i].era);
            encontrados++;
        }
    }
}

void excluir(Personagem vet[], int *qtd)
{
    char nomeBusca[50];
    printf("\nNome do personagem para excluir: ");
    lerString(nomeBusca, sizeof(nomeBusca));

    for (int i = 0; i < *qtd; i++)
    {
        if (stricmp(vet[i].nome, nomeBusca) == 0)
        {
            printf("Nome: %s\n", vet[i].nome);
            printf("Classe: %s\n", vet[i].classe);
            printf("Poder: %s\n", vet[i].poder);
            printf("Era: %s\n", vet[i].era);
            printf("Alinhamento: %s\n", vet[i].alinhamento);

            char confirm[4];
            printf("Tem certeza que deseja excluir este personagem? (sim/nao): ");
            lerString(confirm, sizeof(confirm));
            if (confirm[0] == 's' || confirm[0] == 'S')
            {
                for (int j = i; j < *qtd - 1; j++)
                {
                    vet[j] = vet[j + 1];
                }
                (*qtd)--;
                printf("Personagem excluido com sucesso.\n");
            }
            else
            {
                printf("Exclusao cancelada.\n");
            }
            return;
        }
    }
    printf("Personagem nao encontrado.\n");
}

void salvar(Personagem vet[], int qtd)
{
    FILE *f = fopen("catalogo.txt", "w");

    if (!f)
    {
        printf("Erro ao salvar arquivo!\n");
        return;
    }

    for (int i = 0; i < qtd; i++)
    {
        fprintf(f, "%s;%s;%s;%s;%s\n",
                vet[i].nome,
                vet[i].classe,
                vet[i].poder,
                vet[i].era,
                vet[i].alinhamento);
    }

    fclose(f);
    printf("Dados salvos com sucesso!\n");
}

void carregar(Personagem vet[], int *qtd)
{
    FILE *f = fopen("catalogo.txt", "r");
    if (!f)
        return;

    while (*qtd < MAX)
    {
        int r = fscanf(f, "%49[^;];%19[^;];%99[^;];%19[^;];%19[^\n]\n",
                       vet[*qtd].nome,
                       vet[*qtd].classe,
                       vet[*qtd].poder,
                       vet[*qtd].era,
                       vet[*qtd].alinhamento);
        if (r != 5)
            break;
        (*qtd)++;
    }

    fclose(f);
}

void editar(Personagem vet[], int qtd)
{
    char nomeBusca[50];
    printf("\nNome do personagem para Editar: ");
    lerString(nomeBusca, sizeof(nomeBusca));

    for (int i = 0; i < qtd; i++)
    {
        if (stricmp(vet[i].nome, nomeBusca) == 0)
        {
            char tmp[200];

            printf("-- Edicao --\n");

            printf("Novo nome (Enter para manter): ");
            lerString(tmp, sizeof(tmp));

            if (strlen(tmp) > 0)
            {
                // Verificar duplicado
                if (existeNome(vet, qtd, tmp) && stricmp(tmp, vet[i].nome) != 0)
                {
                    printf("Erro: Já existe outro personagem com esse nome!\n");
                    return;
                }

                strncpy(vet[i].nome, tmp, sizeof(vet[i].nome) - 1);
            }

            printf("Nova classe (Enter para manter): ");
            lerString(tmp, sizeof(tmp));
            if (strlen(tmp) > 0)
                strncpy(vet[i].classe, tmp, sizeof(vet[i].classe) - 1);

            printf("Novo poder (Enter para manter): ");
            lerString(tmp, sizeof(tmp));
            if (strlen(tmp) > 0)
                strncpy(vet[i].poder, tmp, sizeof(vet[i].poder) - 1);

            printf("Nova era (Enter para manter): ");
            lerString(tmp, sizeof(tmp));
            if (strlen(tmp) > 0)
                strncpy(vet[i].era, tmp, sizeof(vet[i].era) - 1);

            printf("Novo alinhamento (Enter para manter): ");
            lerString(tmp, sizeof(tmp));
            if (strlen(tmp) > 0)
                strncpy(vet[i].alinhamento, tmp, sizeof(vet[i].alinhamento) - 1);

            // Garantir null terminator
            vet[i].nome[sizeof(vet[i].nome) - 1] = '\0';
            vet[i].classe[sizeof(vet[i].classe) - 1] = '\0';
            vet[i].poder[sizeof(vet[i].poder) - 1] = '\0';
            vet[i].era[sizeof(vet[i].era) - 1] = '\0';
            vet[i].alinhamento[sizeof(vet[i].alinhamento) - 1] = '\0';

            printf("Personagem atualizado!\n");
            return;
        }
    }

    printf("Personagem nao encontrado.\n");
}

int compara_nomes(const void *a, const void *b)
{
    Personagem *pa = (Personagem *)a;
    Personagem *pb = (Personagem *)b;
    return stricmp(pa->nome, pb->nome);
}

void listarOrdenado(Personagem vet[], int qtd)
{
    Personagem TEMP[MAX];
    memcpy(TEMP, vet, qtd * sizeof(Personagem));
    qsort(TEMP, qtd, sizeof(Personagem), compara_nomes);
    listar(TEMP, qtd);
}

int existenome(Personagem vet[], int qtd, const char *nome)
{
    for (int i = 0; i < qtd; i++)
    {
        if (stricmp(vet[i].nome, nome) == 0)
        {
            return 1;
        }
    }
    return 0;
}
