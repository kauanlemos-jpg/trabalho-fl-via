
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define ARQUIVO_BIN "agendamentos.dat"
#define ARQUIVO_TXT "agendamentos_exportados.txt"

typedef struct {
    int id;
    char paciente[50];
    char medico[50];
    char data[11];
    char hora[6];
    char status;
} Agendamento;

// CADASTRAR
void cadastrar() {

    FILE *fp = fopen(ARQUIVO_BIN, "ab+");

    if (!fp) {
        printf("Erro ao abrir arquivo.\n");
        system("pause");
        return;
    }

    Agendamento ag, temp;

    printf("\n--- Novo Agendamento ---\n");

    printf("ID: ");
    scanf("%d", &ag.id);

    // Verificar ID duplicado
    rewind(fp);

    while (fread(&temp, sizeof(Agendamento), 1, fp)) {
        if (temp.id == ag.id) {
            printf("ID ja cadastrado.\n");
            fclose(fp);
            system("pause");
            return;
        }
    }

    printf("Nome do Paciente: ");
    scanf(" %49[^\n]", ag.paciente);

    printf("Nome do Medico: ");
    scanf(" %49[^\n]", ag.medico);

    printf("Data (DD/MM/AAAA): ");
    scanf(" %10[^\n]", ag.data);

    printf("Hora (HH:MM): ");
    scanf(" %5[^\n]", ag.hora);

    ag.status = 'A';

    if (fwrite(&ag, sizeof(Agendamento), 1, fp) == 1) {
        printf("Agendamento cadastrado com sucesso!\n");
    } else {
        printf("Erro ao cadastrar agendamento.\n");
    }

    fclose(fp);
    system("pause");
}

// CONSULTAR
void consultar() {

    FILE *fp = fopen(ARQUIVO_BIN, "rb");

    if (!fp) {
        printf("Nenhum agendamento encontrado.\n");
        system("pause");
        return;
    }

    int id, encontrado = 0;

    printf("\n--- Consultar Agendamento ---\n");
    printf("Digite o ID: ");
    scanf("%d", &id);

    Agendamento ag;

    while (fread(&ag, sizeof(Agendamento), 1, fp)) {

        if (ag.id == id && ag.status == 'A') {

            printf("\nPaciente: %s", ag.paciente);
            printf("\nMedico: %s", ag.medico);
            printf("\nData: %s", ag.data);
            printf("\nHora: %s\n", ag.hora);

            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Agendamento nao encontrado ou cancelado.\n");
    }

    fclose(fp);
    system("pause");
}

// EXPORTAR PARA TXT
void exportarTXT() {

    FILE *bin = fopen(ARQUIVO_BIN, "rb");

    if (!bin) {
        printf("Nenhum agendamento encontrado.\n");
        system("pause");
        return;
    }

    FILE *txt = fopen(ARQUIVO_TXT, "w");

    if (!txt) {
        printf("Erro ao criar arquivo TXT.\n");
        fclose(bin);
        system("pause");
        return;
    }

    Agendamento ag;

    fprintf(txt, "--- RELATORIO DE AGENDAMENTOS ---\n\n");

    while (fread(&ag, sizeof(Agendamento), 1, bin)) {

        if (ag.status == 'A') {

            fprintf(txt,
                "ID: %d | Paciente: %s | Medico: %s | Data: %s | Hora: %s\n",
                ag.id,
                ag.paciente,
                ag.medico,
                ag.data,
                ag.hora);
        }
    }

    fclose(bin);
    fclose(txt);

    printf("Dados exportados para %s com sucesso!\n", ARQUIVO_TXT);

    system("pause");
}

// EXCLUIR / CANCELAR
void excluir() {

    FILE *fp = fopen(ARQUIVO_BIN, "rb+");

    if (!fp) {
        printf("Arquivo nao encontrado.\n");
        system("pause");
        return;
    }

    int id, encontrado = 0;

    printf("\n--- Cancelar Agendamento ---\n");
    printf("Digite o ID: ");
    scanf("%d", &id);

    Agendamento ag;

    while (fread(&ag, sizeof(Agendamento), 1, fp)) {

        if (ag.id == id && ag.status == 'A') {

            ag.status = 'E';

            fseek(fp, -(long)sizeof(Agendamento), SEEK_CUR);

            if (fwrite(&ag, sizeof(Agendamento), 1, fp) == 1) {
                printf("Agendamento cancelado com sucesso.\n");
            } else {
                printf("Erro ao atualizar agendamento.\n");
            }

            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Agendamento nao encontrado ou ja cancelado.\n");
    }

    fclose(fp);
    system("pause");
}

// PROGRAMA PRINCIPAL
int main() {

    setlocale(LC_ALL, "Portuguese");

    int op;

    do {

        system("cls");

        printf("--- SISTEMA DE AGENDAMENTO MEDICO ---\n");
        printf("\n1. Cadastrar Agendamento");
        printf("\n2. Consultar por ID");
        printf("\n3. Exportar para TXT");
        printf("\n4. Cancelar Agendamento");
        printf("\n0. Sair");

        printf("\n\nEscolha uma opcao: ");
        scanf("%d", &op);

        switch (op) {

            case 1:
                cadastrar();
                break;

            case 2:
                consultar();
                break;

            case 3:
                exportarTXT();
                break;

            case 4:
                excluir();
                break;

            case 0:
                printf("Encerrando o sistema...\n");
                break;

            default:
                printf("Opcao invalida!\n");
                system("pause");
                break;
        }

    } while (op != 0);

    return 0;
}

