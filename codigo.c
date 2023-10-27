#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_NAME_LENGTH 50
#define MAX_EMAIL_LENGTH 50
#define MAX_PHONE_LENGTH 15
#define MAX_INGRESSOS 20
#define CODIGO_LENGTH 4

struct User {
    char nome[MAX_NAME_LENGTH];
    int idade;
    char email[MAX_EMAIL_LENGTH];
    char telefone[MAX_PHONE_LENGTH];
};

/*Função para escrever os dados do usuário em um arquivo CSV com colunas separadas.
função void não retorna nenhum valor.
função FILE representa os arquivos.
função fopen é usada para abrir um arquivo, recebe o nome do arquivo e o modo de abertura.
NULL não aponta para nenhum local de memoria valido
*/
void escreverDadosParaCSV(struct User user) {
    FILE *arquivoCSV = fopen("dados_usuario.csv", "w");
    if (arquivoCSV == NULL) {
        printf("Erro ao abrir o arquivo CSV.\n");
        return;
    }

    // Escrever os cabeçalhos das colunas
    // fprintf imprime na tela dados formatados em um arquivo, no caso csv
    fprintf(arquivoCSV, "Nome,Idade\n");
    
    // Escrever os dados do usuário em colunas separadas
    fprintf(arquivoCSV, "%s,%d\n", user.nome, user.idade);

    // fclose retorna um valor inteiro que indica se o fechamento do arquivo foi bem sucedido
    fclose(arquivoCSV);
    printf("Dados do usuario foram salvos no arquivo dados_usuario.csv.\n");
}

void qtdingresso(int quantidade, char nomes[][MAX_NAME_LENGTH]) {
    printf("Por favor, digite o nome de cada visitante:\n");

    for (int i = 0; i < quantidade; i++) {
        printf("Digite o nome do visitante %d:", i + 1);
        if (i == 0) getchar();
         // Limpar o buffer apenas na primeira interação

        fgets(nomes[i], MAX_NAME_LENGTH, stdin);
        // para conseguir ler nomes completos
    }
}

void gerarCodigos(int quantidade, char nomes[][MAX_NAME_LENGTH]) {
    printf("| Ingressos gerados: |\n");
    srand(time(NULL)); 
    // Inicializa com o tempo atual

    for (int i = 0; i < quantidade; i++) {
        char codigo[CODIGO_LENGTH + 1];
        // +1 para o caractere nulo no final

        for (int j = 0; j < CODIGO_LENGTH; j++) {
            codigo[j] = '0' + rand() % 10;
            // rand Gera dígitos de 0 a 9
        }
        codigo[CODIGO_LENGTH] = '\0';
        printf("%s - Ingresso %s\n", nomes[i], codigo);
    }
}

int obterPeriodoVisita() {
    int opcao;
    printf("Periodo da manha: 7:00 as 12:00 | Periodo da tarde: 13:00 as 18:00\n");
    printf("Escolha a opcao desejada:\n");
    printf("Periodo da manha Digite [1]\n");
    printf("Periodo da tarde Digite [2]\n");
    scanf("%d", &opcao);

    // Validar a opção
    if (opcao != 1 && opcao != 2) {
        printf("Opção invalida. Por favor, escolha 1 ou 2.\n");
        return obterPeriodoVisita();
    }

    return opcao;
}

int validarIngresso(int horarioEscolhido) {
    time_t agora;
    struct tm *infoTempo;

    time(&agora);
    infoTempo = localtime(&agora);

    int horaAtual = infoTempo->tm_hour;

    if ((horarioEscolhido == 1 && horaAtual >= 7 && horaAtual <= 12) ||
        (horarioEscolhido == 2 && horaAtual >= 13 && horaAtual <= 18)) {
        return 1; 
        // Ingresso válido
    } else {
        return 0; 
        // Ingresso expirado
    }
}

void validarIngressosDigitados(int quantidade, int periodoVisita) {
    char ingressos[MAX_INGRESSOS][CODIGO_LENGTH + 1];
    bool algumIngressoExpirado = false; 
    // Variável para indicar se algum ingresso expirou

    printf("|||||| VALIDACAO DOS INGRESSOS |||||\n");
    printf("Por favor, digite o numero de cada ingresso adquirido:\n");

    for (int i = 0; i < quantidade; i++) {
        printf("Ingresso %d: ", i + 1);
        scanf("%s", ingressos[i]);
    }

    printf("Validacao dos ingressos digitados:\n");

    for (int i = 0; i < quantidade; i++) {
        int ingressoValido = validarIngresso(periodoVisita);
        if (ingressoValido) {
            printf("Ingresso %s validado. Aproveite e tenha uma otima visita.\n", ingressos[i]);
        } else {
            printf("Ingresso %s expirado. Adquira outros ingressos com o horario desejado para que possa realizar a visita.\n", ingressos[i]);
            algumIngressoExpirado = true; 
            // Marca que pelo menos um ingresso expirou
        }
    }

    if (!algumIngressoExpirado) {
        printf("Aproveite e tenha uma otima visita.\n");
    }
}

int main() {
    struct User user;
    int quantidade;
    int periodoVisita;

    printf("Cadastro de Usuario\n");

    printf("Nome:\n");
    fgets(user.nome, MAX_NAME_LENGTH, stdin);
    user.nome[strcspn(user.nome, "\n")] = '\0'; 
    // Remover o caractere de nova linha

    printf("E-mail:\n");
    fgets(user.email, MAX_EMAIL_LENGTH, stdin);
    user.email[strcspn(user.email, "\n")] = '\0'; 
    // Remover o caractere de nova linha

    printf("Telefone/Celular:\n");
    fgets(user.telefone, MAX_PHONE_LENGTH, stdin);
    user.telefone[strcspn(user.telefone, "\n")] = '\0'; 
    // Remover o caractere de nova linha

    printf("Idade:\n");
    scanf("%d", &user.idade);

    if (user.idade < 18) {
        printf("Desculpe, compra de ingressos a partir dos 18 anos.\n");
        return 1;
    } else {
        printf("Bem-vindo(a)! Prossiga com sua compra...\n");
    }

    // Pergunta sobre o período de visita
    printf("Qual periodo deseja visitar o Museu das Favelas?\n");
    periodoVisita = obterPeriodoVisita();

    printf("Selecione a quantidade de ingressos (max. 5):\n");
    scanf("%d", &quantidade);

    if (quantidade < 1 || quantidade > MAX_INGRESSOS) {
        printf("Quantidade de ingressos invalida.\n");
        return 1;
    }

    char nomes[MAX_INGRESSOS][MAX_NAME_LENGTH];
    qtdingresso(quantidade, nomes);

    printf("Informacoes do Usuario cadastradas:\n");
    printf("Nome: %s\n", user.nome);
    printf("E-mail: %s\n", user.email);
    printf("Telefone/Celular: %s\n", user.telefone);
    printf("Idade: %d\n", user.idade);

    // Adicionar lógica para exibir o período selecionado
    printf("Periodo de visita: %s\n", (periodoVisita == 1) ? "Manha" : "Tarde");

    gerarCodigos(quantidade, nomes);

    // Adicione esta parte para validar os ingressos digitados
    validarIngressosDigitados(quantidade, periodoVisita);

    // Chame a função para escrever os dados do usuário em um arquivo CSV no final do programa
    // após todas as outras operações.
    // Nesse ponto, user contém as informações preenchidas pelo usuário.
    escreverDadosParaCSV(user);

    return 0;
}