#include <logger.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

// LOG COLORS
#define TRACE_COLOR "\e[0;37m"          // White Color
#define INFO_COLOR  "\e[0;32m"          // Green Color
#define DEBUG_COLOR "\e[0;34m"          // Blue Color
#define WARN_COLOR  "\e[0;33m"          // Yellow Color
#define ERROR_COLOR "\e[1;91m"          // Bold Red Text
#define CRITICAL_COLOR "\e[1;90m\e[41m"    // Bold Black Text, Red Background
#define RESET       "\e[0m"             // Reset All

namespace RSCV
{
    // Matriz que indica a cor e o texto inicial para cada tipo de mensagem
    static const char* s_LevelStrings[6][2] = { 
        {TRACE_COLOR, "[TRACE]: "},
        {INFO_COLOR, "[INFO]: "}, 
        {DEBUG_COLOR, "[DEBUG]: "}, 
        {WARN_COLOR, "[WARN]: "}, 
        {ERROR_COLOR, "[ERROR]: "}, 
        {CRITICAL_COLOR, "[CRITICAL]: "}
    };

    void Logger::Log(LogType type, const char* format, ...)
    {
        // Precisa existir uma cópia pois após usar o vsnprintf
        // A lista se torna inválida
        va_list argsList, argsCopy;
        
        va_start(argsList, format); // Inicia a lista de argumentos variados
        va_copy(argsCopy, argsList); // Copia antes da primeira leitura

        size_t bufferSize = vsnprintf(nullptr, 0, format, argsCopy); // Verifica o tamanho necessário para a string formatada
        va_end(argsCopy); // Termina a cópia da lista

        if (bufferSize > 0)
        {
            char *contentBuffer = (char*)malloc(bufferSize + 1); // Aloca o tamanho necessário

            if (contentBuffer != nullptr)
            {
                time_t t;
                struct tm *timeInfo;
                char *dateStr, timeStr[9]; // String da data e String das horas minutos e segundos
                
                time(&t);
                // Obtém o tempo atual em dia da semana, mês, horas, minutos, segundos e ano
                timeInfo = localtime(&t); 
                dateStr = asctime(timeInfo); // Transforma para string
                
                strncpy(timeStr, dateStr + 11, 8); // Copia apenas as horas, minutos e segundos
                timeStr[8] = '\0'; // Último caracter deve ser o nulo

                vsnprintf(contentBuffer, bufferSize + 1, format, argsList); // Coloca a string formatada no buffer

                // Printa o texto no formato -> {DEFINE_COR} [hh:mm:ss][TIPO]: MENSAGEM FORMATADA {RESETA_COR}
                printf("%s[%s]%s%s%s\n", s_LevelStrings[type][0], timeStr, s_LevelStrings[type][1], contentBuffer, RESET);
                
                free(contentBuffer); // Liberando a memória do buffer
            }
        }

        va_end(argsList); // Termina a lista de argumentos variados
    }
}