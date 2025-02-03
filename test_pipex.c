#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PIPEX_EXEC "./pipex"  // Укажите путь к исполняемому файлу pipex

// Функция для выполнения теста с сравнением через diff
void run_pipex_test(const char *input, const char *cmd1, const char *cmd2, const char *test_name) {
    char pipex_output[] = "pipex_output.txt";
    char shell_output[] = "shell_output.txt";
    char command[512];

    // Подготовка входного файла (НЕ удаляем перед проверкой памяти)
    FILE *input_file = fopen("test_input.txt", "w");
    if (!input_file) {
        perror("Ошибка создания входного файла");
        exit(EXIT_FAILURE);
    }
    fprintf(input_file, "%s", input);
    fclose(input_file);

    // Ожидаемый результат через шелл
    snprintf(command, sizeof(command), "< test_input.txt %s | %s > %s", cmd1, cmd2, shell_output);
    system(command);

    // Выполняем pipex
    snprintf(command, sizeof(command), "%s test_input.txt \"%s\" \"%s\" %s", PIPEX_EXEC, cmd1, cmd2, pipex_output);
    printf("\n[ТЕСТ] %s\n", test_name);
    printf("Команда: %s\n", command);

    int result = system(command);
    if (result != 0) {
        printf("[❌] Ошибка выполнения pipex\n");
        return;
    }

    // Сравнение файлов с помощью diff
    snprintf(command, sizeof(command), "diff %s %s", pipex_output, shell_output);
    if (system(command) == 0) {
        printf("[✔] Тест пройден: %s\n", test_name);
    } else {
        printf("[✖] Ошибка в тесте: %s\n", test_name);
        printf("Ожидаемый результат:\n");
        system("cat shell_output.txt");
        printf("Результат pipex:\n");
        system("cat pipex_output.txt");
    }
}

// Функция для проверки утечек памяти с Valgrind (НЕ удаляем test_input.txt перед запуском)
void run_memory_test(const char *cmd1, const char *cmd2) {
    char command[512];

    snprintf(command, sizeof(command), "valgrind --leak-check=full --track-origins=yes %s test_input.txt \"%s\" \"%s\" test_output.txt", PIPEX_EXEC, cmd1, cmd2);
    printf("\n[🔍] Проверка утечек памяти:\n%s\n", command);
    system(command);
}

int main() {
    // Тесты с прямым сравнением оригинала и pipex
    run_pipex_test("file1\nfile2\n", "ls -l", "wc -l", "Базовый тест");
    run_pipex_test("Hello Pipex\nTest\n", "grep Pipex", "wc -l", "Фильтрация grep");
    run_pipex_test("", "cat", "wc -l", "Пустой входной файл");

    // Проверка памяти (запускаем после всех тестов, НЕ удаляя test_input.txt)
    run_memory_test("ls -l", "wc -l");

    // Очистка временных файлов ПОСЛЕ завершения всех тестов
    remove("test_input.txt");
    remove("pipex_output.txt");
    remove("shell_output.txt");

    return 0;
}
