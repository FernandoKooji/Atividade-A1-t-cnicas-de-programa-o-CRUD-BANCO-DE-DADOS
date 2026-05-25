
/*
 * lista_emails_portavel.c
 * Compatível com Linux e Windows
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_NOME 100
#define TAM_EMAIL 150
#define TAM_DATA 11
#define TAM_HORA 9

typedef struct {
    int id;
    char nome[TAM_NOME];
    char email[TAM_EMAIL];
    char data[TAM_DATA];
    char hora[TAM_HORA];
} Registro;

typedef struct No {
    Registro reg;
    struct No *prox;
} No;

typedef struct {
    No *cabeca;
    int tamanho;
    int proximo_id;
} Lista;


/* ================= PROTÓTIPOS ================= */

int validar_nome(const char *nome);
int validar_email(const char *email);
int bissexto(int ano);
int validar_data(const char *data);
int validar_hora(const char *hora);

void exibir_registro(const No *n);

void buscar_nome(Lista *l, const char *nome);
void buscar_data(Lista *l, const char *data);
void menu_busca(Lista *l);

int cadastrar_manual(Lista *l);

int remover_registro(Lista *l, int id);
void menu_remover(Lista *l);

int exportar_txt(Lista *l, const char *arquivo);

void menu_importacao(Lista *l);
void menu_exportacao(Lista *l);

/* ================= VALIDAÇÕES DE DADOS ================= */

int validar_nome(const char *nome)
{
    int i;

    if (strlen(nome) == 0)
        return 0;

    for (i = 0; nome[i] != '\0'; i++)
    {
        if (!isalpha((unsigned char)nome[i]) &&
            nome[i] != ' ')
        {
            return 0;
        }
    }

    return 1;
}

int validar_email(const char *email)
{
    const char *arroba;
    const char *ponto;

    arroba = strchr(email, '@');

    if (arroba == NULL)
        return 0;

    ponto = strchr(arroba, '.');

    if (ponto == NULL)
        return 0;

    return 1;
}

int bissexto(int ano)
{
    if ((ano % 4 == 0 && ano % 100 != 0) ||
        (ano % 400 == 0))
    {
        return 1;
    }

    return 0;
}

int validar_data(const char *data)
{
    int d, m, a;

    if (sscanf(data, "%d/%d/%d", &d, &m, &a) != 3)
        return 0;

    if (m < 1 || m > 12)
        return 0;

    int dias_mes[] =
    {
        31,28,31,30,31,30,
        31,31,30,31,30,31
    };

    if (bissexto(a))
        dias_mes[1] = 29;

    if (d < 1 || d > dias_mes[m - 1])
        return 0;

    return 1;
}

int validar_hora(const char *hora)
{
    int h, m, s;

    if (sscanf(hora, "%d:%d:%d", &h, &m, &s) != 3)
        return 0;

    if (h < 0 || h > 23)
        return 0;

    if (m < 0 || m > 59)
        return 0;

    if (s < 0 || s > 59)
        return 0;

    return 1;
}

/* ================= BUSCA ================= */

void buscar_nome(Lista *l, const char *nome)
{
    No *cur = l->cabeca;

    int encontrados = 0;

    while (cur != NULL)
    {
        if (strstr(cur->reg.nome, nome) != NULL)
        {
            exibir_registro(cur);

            encontrados++;
        }

        cur = cur->prox;
    }

    if (encontrados == 0)
    {
        printf("\nNenhum registro encontrado.\n");
    }
    else
    {
        printf("\n%d registro(s) encontrado(s).\n", encontrados);
    }
}

void buscar_data(Lista *l, const char *data)
{
    No *cur = l->cabeca;

    int encontrados = 0;

    while (cur != NULL)
    {
        if (strcmp(cur->reg.data, data) == 0)
        {
            exibir_registro(cur);

            encontrados++;
        }

        cur = cur->prox;
    }

    if (encontrados == 0)
    {
        printf("\nNenhum registro encontrado.\n");
    }
    else
    {
        printf("\n%d registro(s) encontrado(s).\n", encontrados);
    }
}

/* ================= REMOVER REGISTRO ================= */

int remover_registro(Lista *l, int id)
{
    No *cur = l->cabeca;

    No *ant = NULL;

    while (cur != NULL)
    {
        if (cur->reg.id == id)
        {
            /* remove primeiro nó */
            if (ant == NULL)
            {
                l->cabeca = cur->prox;
            }
            else
            {
                ant->prox = cur->prox;
            }

            free(cur);

            l->tamanho--;

            printf("\nRegistro removido com sucesso.\n");

            return 1;
        }

        ant = cur;

        cur = cur->prox;
    }

    printf("\nID nao encontrado.\n");

    return 0;
}

/* ================= UTILITÁRIOS ================= */

void listar_txt() {

    printf("\nArquivos TXT disponiveis:\n\n");

#ifdef _WIN32
    system("dir /b *.txt");
#else
    system("ls *.txt 2>/dev/null");
#endif
}

void listar_bin() {

    printf("\nArquivos BIN disponiveis:\n\n");

#ifdef _WIN32
    system("dir /b *.bin");
#else
    system("ls *.bin 2>/dev/null");
#endif
}

void listar_csv() {

    printf("\nArquivos CSV disponiveis:\n\n");

#ifdef _WIN32
    system("dir /b *.csv");
#else
    system("ls *.csv 2>/dev/null");
#endif
}

void trim(char *s) {

    if (!s)
        return;

    int ini = 0;

    while (s[ini] && isspace((unsigned char)s[ini]))
        ini++;

    if (ini)
        memmove(s, s + ini, strlen(s) - ini + 1);

    int fim = strlen(s) - 1;

    while (fim >= 0 && isspace((unsigned char)s[fim]))
        s[fim--] = '\0';
}

int ler_linha(char *buf, int max) {

    if (!fgets(buf, max, stdin))
        return 0;

    buf[strcspn(buf, "\n")] = '\0';

    trim(buf);

    return 1;
}

/* ================= LISTA ================= */

void lista_init(Lista *l) {

    l->cabeca = NULL;
    l->tamanho = 0;
    l->proximo_id = 1;
}

No *criar_no(const Registro *r) {

    No *novo = (No *) malloc(sizeof(No));

    if (!novo) {
        printf("Erro de memoria.\n");
        return NULL;
    }

    novo->reg = *r;
    novo->prox = NULL;

    return novo;
}

int inserir(Lista *l, Registro *r) {

    r->id = l->proximo_id;

    No *novo = criar_no(r);

    if (!novo)
        return 0;

    if (!l->cabeca) {

        l->cabeca = novo;
    }
    else {

        No *cur = l->cabeca;

        while (cur->prox)
            cur = cur->prox;

        cur->prox = novo;
    }

    l->proximo_id++;
    l->tamanho++;

    return 1;
}

void exibir_registro(const No *n) {

    printf("\n[ID %d]\n", n->reg.id);
    printf("Nome : %s\n", n->reg.nome);
    printf("Email: %s\n", n->reg.email);
    printf("Data : %s\n", n->reg.data);
    printf("Hora : %s\n", n->reg.hora);
}

void listar_registros(const Lista *l) {

    if (!l->cabeca) {

        printf("\nLista vazia.\n");
        return;
    }

    No *cur = l->cabeca;

    while (cur) {

        exibir_registro(cur);

        cur = cur->prox;
    }

    printf("\nTotal: %d registro(s).\n", l->tamanho);
}

void liberar_lista(Lista *l) {

    No *cur = l->cabeca;

    while (cur) {

        No *tmp = cur->prox;

        free(cur);

        cur = tmp;
    }

    l->cabeca = NULL;
    l->tamanho = 0;
}

/* ================= CADASTRO MANUAL ================= */

int cadastrar_manual(Lista *l)
{
    Registro r;

    char entrada[256];

    memset(&r, 0, sizeof(r));

    printf("\nDigite CANCELAR para voltar ao menu.\n");

    /* NOME */

    while (1)
    {
        printf("\nNome: ");

        ler_linha(entrada, sizeof(entrada));

        if (strcmp(entrada, "cancelar") == 0)
            return 0;

        if (!validar_nome(entrada))
        {
            printf("Nome invalido.\n");
            continue;
        }

        strcpy(r.nome, entrada);

        break;
    }

    /* EMAIL */

    while (1)
    {
        printf("Email: ");

        ler_linha(entrada, sizeof(entrada));

        if (strcmp(entrada, "cancelar") == 0)
            return 0;

        if (!validar_email(entrada))
        {
            printf("Email invalido.\n");
            continue;
        }

        strcpy(r.email, entrada);

        break;
    }

    /* DATA */

    while (1)
    {
        printf("Data (DD/MM/AAAA): ");

        ler_linha(entrada, sizeof(entrada));

        if (strcmp(entrada, "cancelar") == 0)
            return 0;

        if (!validar_data(entrada))
        {
            printf("Data invalida.\n");
            continue;
        }

        strcpy(r.data, entrada);

        break;
    }

    /* HORA */

    while (1)
    {
        printf("Hora (HH:MM:SS): ");

        ler_linha(entrada, sizeof(entrada));

        if (strcmp(entrada, "cancelar") == 0)
            return 0;

        if (!validar_hora(entrada))
        {
            printf("Hora invalida.\n");
            continue;
        }

        strcpy(r.hora, entrada);

        break;
    }

    inserir(l, &r);

    printf("\nRegistro cadastrado com sucesso.\n");

    return 1;
}

/* ================= IMPORTAR TXT ================= */

static const char *extrair_campo(
    const char *p,
    const char *prefixo,
    char *dest,
    int maxdest
) {

    size_t len = strlen(prefixo);

    if (strncmp(p, prefixo, len) != 0)
        return NULL;

    p += len;

    int i = 0;

    while (*p && *p != ';' && i < maxdest - 1)
        dest[i++] = *p++;

    dest[i] = '\0';

    trim(dest);

    if (*p == ';')
        p++;

    while (*p == ' ')
        p++;

    return p;
}

int importar_txt(Lista *l, const char *arquivo) {

    FILE *f = fopen(arquivo, "r");

    if (!f) {

        printf("\nErro ao abrir TXT.\n");
        return 0;
    }

    char linha[512];

    int importados = 0;

    while (fgets(linha, sizeof(linha), f)) {

        linha[strcspn(linha, "\n")] = '\0';

        trim(linha);

        if (linha[0] == '#' || linha[0] == '\0')
            continue;

        Registro r;

        memset(&r, 0, sizeof(r));

        const char *p = linha;

        p = extrair_campo(p, "NOME: ", r.nome, TAM_NOME);
        if (!p) continue;

        p = extrair_campo(p, "EMAIL: ", r.email, TAM_EMAIL);
        if (!p) continue;

        p = extrair_campo(p, "DATA: ", r.data, TAM_DATA);
        if (!p) continue;

        p = extrair_campo(p, "HORA: ", r.hora, TAM_HORA);
        if (!p) continue;

        inserir(l, &r);

        importados++;
    }

    fclose(f);

    printf("\n%d registro(s) importado(s).\n", importados);

    return importados;
}

/* ================= EXPORTAR TXT ================= */

int exportar_txt(Lista *l, const char *arquivo)
{
    FILE *arq;

    arq = fopen(arquivo, "w");

    if (arq == NULL)
    {
        printf("\nErro ao criar TXT.\n");
        return 0;
    }

    fprintf(arq, "# HEADER: LOG_EMAIL_V1\n");

    No *cur = l->cabeca;

    int total = 0;

    while (cur != NULL)
    {
        fprintf(
            arq,
            "NOME: %s; EMAIL: %s; DATA: %s; HORA: %s\n",
            cur->reg.nome,
            cur->reg.email,
            cur->reg.data,
            cur->reg.hora
        );

        total++;

        cur = cur->prox;
    }

    fclose(arq);

    printf("\n%d registro(s) exportado(s) em TXT.\n", total);

    return total;
}

/* ================= EXPORTAR BIN ================= */

int exportar_binario(Lista *l, const char *arquivo)
{
    FILE *arq;

    int result;
    int total = 0;

    arq = fopen(arquivo, "wb");

    if (arq == NULL)
    {
        printf("\nProblemas na CRIACAO do arquivo BIN.\n");
        return 0;
    }

    No *cur = l->cabeca;

    while (cur != NULL)
    {
        result = fwrite(
                    &cur->reg,
                    sizeof(Registro),
                    1,
                    arq
                 );

        if (result != 1)
        {
            printf("\nErro na gravacao binaria.\n");

            fclose(arq);

            return 0;
        }

        total++;

        cur = cur->prox;
    }

    fclose(arq);

    printf("\n%d registro(s) exportado(s) em BIN.\n", total);

    return total;
}

/* ================= RECUPERAR BIN ================= */

int recuperar_binario(Lista *l, const char *arquivo)
{
    FILE *arq;

    arq = fopen(arquivo, "rb");

    if (arq == NULL)
    {
        printf("\nErro ao abrir BIN.\n");
        return 0;
    }

    liberar_lista(l);

    Registro r;

    int recuperados = 0;

    while (fread(&r, sizeof(Registro), 1, arq) == 1)
    {
        No *novo = criar_no(&r);

        if (!novo)
        {
            fclose(arq);
            return 0;
        }

        if (l->cabeca == NULL)
        {
            l->cabeca = novo;
        }
        else
        {
            No *cur = l->cabeca;

            while (cur->prox)
                cur = cur->prox;

            cur->prox = novo;
        }

        if (r.id >= l->proximo_id)
            l->proximo_id = r.id + 1;

        l->tamanho++;

        recuperados++;
    }

    fclose(arq);

    printf("\n%d registro(s) recuperado(s).\n", recuperados);

    return recuperados;
}

/* ================= EXPORTAR CSV ================= */

int exportar_csv(Lista *l, const char *arquivo)
{
    FILE *arq;

    arq = fopen(arquivo, "w");

    if (arq == NULL)
    {
        printf("\nErro ao criar CSV.\n");
        return 0;
    }

    /* cabeçalho */
    fprintf(arq, "ID,NOME,EMAIL,DATA,HORA\n");

    No *cur = l->cabeca;

    int total = 0;

    while (cur != NULL)
    {
        fprintf(
            arq,
            "%d,%s,%s,%s,%s\n",
            cur->reg.id,
            cur->reg.nome,
            cur->reg.email,
            cur->reg.data,
            cur->reg.hora
        );

        total++;

        cur = cur->prox;
    }

    fclose(arq);

    printf("\n%d registro(s) exportado(s) em CSV.\n", total);

    return total;
}

/* ================= RECUPERAR CSV ================= */

int recuperar_csv(Lista *l, const char *arquivo)
{
    FILE *arq;

    arq = fopen(arquivo, "r");

    if (arq == NULL)
    {
        printf("\nErro ao abrir CSV.\n");
        return 0;
    }

    liberar_lista(l);

    char linha[512];

    /* ignora cabeçalho */
    fgets(linha, sizeof(linha), arq);

    int total = 0;

    while (fgets(linha, sizeof(linha), arq))
    {
        Registro r;

        memset(&r, 0, sizeof(r));

        sscanf(
            linha,
            "%d,%99[^,],%149[^,],%10[^,],%8[^\n]",
            &r.id,
            r.nome,
            r.email,
            r.data,
            r.hora
        );

        No *novo = criar_no(&r);

        if (novo == NULL)
        {
            fclose(arq);
            return 0;
        }

        if (l->cabeca == NULL)
        {
            l->cabeca = novo;
        }
        else
        {
            No *cur = l->cabeca;

            while (cur->prox)
                cur = cur->prox;

            cur->prox = novo;
        }

        if (r.id >= l->proximo_id)
            l->proximo_id = r.id + 1;

        l->tamanho++;

        total++;
    }

    fclose(arq);

    printf("\n%d registro(s) recuperado(s) do CSV.\n", total);

    return total;
}

/* ================= MENUS ================= */

void menu_importacao(Lista *l)
{
    char op[8];

    char arquivo[256];

    printf("\n====================\n");
    printf(" MENU IMPORTACAO\n");
    printf("====================\n");
    printf("1 - Importar TXT\n");
    printf("2 - Recuperar BIN\n");
    printf("3 - Recuperar CSV\n");
    printf("0 - Voltar\n");
    printf("====================\n");
    printf("Opcao: ");

    ler_linha(op, sizeof(op));

    /* TXT */

    if (strcmp(op, "1") == 0)
    {
        listar_txt();

        printf("\nDigite o nome do TXT: ");

        ler_linha(arquivo, sizeof(arquivo));

        importar_txt(l, arquivo);
    }

    /* BIN */

    else if (strcmp(op, "2") == 0)
    {
        listar_bin();

        printf("\nDigite o nome do BIN: ");

        ler_linha(arquivo, sizeof(arquivo));

        recuperar_binario(l, arquivo);
    }

    /* CSV */

    else if (strcmp(op, "3") == 0)
    {
        listar_csv();

        printf("\nDigite o nome do CSV: ");

        ler_linha(arquivo, sizeof(arquivo));

        recuperar_csv(l, arquivo);
    }

    else if (strcmp(op, "0") == 0)
    {
        return;
    }

    else
    {
        printf("\nOpcao invalida.\n");
    }
}

void menu_exportacao(Lista *l)
{
    char op[8];

    char arquivo[256];

    printf("\n====================\n");
    printf(" MENU EXPORTACAO\n");
    printf("====================\n");
    printf("1 - Exportar TXT\n");
    printf("2 - Exportar BIN\n");
    printf("3 - Exportar CSV\n");
    printf("0 - Voltar\n");
    printf("====================\n");
    printf("Opcao: ");

    ler_linha(op, sizeof(op));

    /* TXT */

    if (strcmp(op, "1") == 0)
    {
        listar_txt();

        printf("\nDigite o nome do TXT: ");

        ler_linha(arquivo, sizeof(arquivo));

        if (strlen(arquivo) == 0)
            strcpy(arquivo, "emails.txt");

        exportar_txt(l, arquivo);
    }

    /* BIN */

    else if (strcmp(op, "2") == 0)
    {
        listar_bin();

        printf("\nDigite o nome do BIN: ");

        ler_linha(arquivo, sizeof(arquivo));

        if (strlen(arquivo) == 0)
            strcpy(arquivo, "emails.bin");

        exportar_binario(l, arquivo);
    }

    /* CSV */

    else if (strcmp(op, "3") == 0)
    {
        listar_csv();

        printf("\nDigite o nome do CSV: ");

        ler_linha(arquivo, sizeof(arquivo));

        if (strlen(arquivo) == 0)
            strcpy(arquivo, "emails.csv");

        exportar_csv(l, arquivo);
    }

    else if (strcmp(op, "0") == 0)
    {
        return;
    }

    else
    {
        printf("\nOpcao invalida.\n");
    }
}

void menu_importar(Lista *l) {

    listar_txt();

    printf("\nDigite o nome do TXT: ");

    char arquivo[256];

    ler_linha(arquivo, sizeof(arquivo));

    importar_txt(l, arquivo);
}

void menu_exportar(Lista *l)
{
    listar_bin();

    char arquivo[256];

    printf("\nDigite o nome do BIN: ");

    ler_linha(arquivo, sizeof(arquivo));

    if (strlen(arquivo) == 0)
        strcpy(arquivo, "emails.bin");

    exportar_binario(l, arquivo);
}

void menu_recuperar(Lista *l)
{
    listar_bin();

    char arquivo[256];

    printf("\nDigite o nome do BIN: ");

    ler_linha(arquivo, sizeof(arquivo));

    recuperar_binario(l, arquivo);
}

void menu_exportar_csv(Lista *l)
{
    listar_csv();

    char arquivo[256];

    printf("\nDigite o nome do CSV: ");

    ler_linha(arquivo, sizeof(arquivo));

    if (strlen(arquivo) == 0)
        strcpy(arquivo, "emails.csv");

    exportar_csv(l, arquivo);
}

void menu_recuperar_csv(Lista *l)
{
    listar_csv();

    char arquivo[256];

    printf("\nDigite o nome do CSV: ");

    ler_linha(arquivo, sizeof(arquivo));

    recuperar_csv(l, arquivo);
}

void menu_busca(Lista *l)
{
    char op[8];

    char busca[100];

    printf("\n====================\n");
    printf(" BUSCA DE REGISTROS\n");
    printf("====================\n");
    printf("1 - Buscar por nome\n");
    printf("2 - Buscar por data\n");
    printf("0 - Voltar\n");
    printf("====================\n");
    printf("Opcao: ");

    ler_linha(op, sizeof(op));

    if (strcmp(op, "1") == 0)
    {
        printf("\nDigite o nome: ");

        ler_linha(busca, sizeof(busca));

        buscar_nome(l, busca);
    }

    else if (strcmp(op, "2") == 0)
    {
        printf("\nDigite a data (DD/MM/AAAA): ");

        ler_linha(busca, sizeof(busca));

        buscar_data(l, busca);
    }

    else if (strcmp(op, "0") == 0)
    {
        return;
    }

    else
    {
        printf("\nOpcao invalida.\n");
    }
}

void menu_remover(Lista *l)
{
    char entrada[20];

    int id;

    printf("\nDigite o ID para remover: ");

    ler_linha(entrada, sizeof(entrada));

    id = atoi(entrada);

    remover_registro(l, id);
}

void exibir_menu() {

    printf("\n==============================\n");
    printf(" GERENCIADOR DE EMAILS\n");
    printf("==============================\n");
    printf("1 - Adicionar registro\n");
    printf("2 - Listar registros\n");
    printf("3 - Buscar registros\n");
    printf("4 - Remover registro\n");
    printf("5 - Importacao\n");
    printf("6 - Exportacao\n");
    printf("0 - Sair\n");
    printf("==============================\n");
    printf("Opcao: ");
}

/* ================= MAIN ================= */

int main() {

    Lista lista;

    lista_init(&lista);

    char op[8];

    int executando = 1;

    while (executando)
    {
        exibir_menu();

        ler_linha(op, sizeof(op));

        /* ADICIONAR */

        if (strcmp(op, "1") == 0)
        {
            cadastrar_manual(&lista);
        }

        /* LISTAR */

        else if (strcmp(op, "2") == 0)
        {
            listar_registros(&lista);
        }

        /* BUSCAR */

        else if (strcmp(op, "3") == 0)
        {
            menu_busca(&lista);
        }

        /* REMOVER */

        else if (strcmp(op, "4") == 0)
        {
            menu_remover(&lista);
        }

        /* IMPORTACAO */

        else if (strcmp(op, "5") == 0)
        {
            menu_importacao(&lista);
        }

        /* EXPORTACAO */

        else if (strcmp(op, "6") == 0)
        {
            menu_exportacao(&lista);
        }

        /* SAIR */

        else if (strcmp(op, "0") == 0)
        {
            executando = 0;
        }

        else
        {
            printf("\nOpcao invalida.\n");
        }
    }

    liberar_lista(&lista);

    printf("\nPrograma encerrado.\n");

    return 0;
}
