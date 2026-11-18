#ifndef GRAFIKA2D_LOGGER_H
#define GRAFIKA2D_LOGGER_H

#include <string>

/**
 * @file Logger.h
 * @brief Zapis komunikatów i błędów do pliku dziennika.
 */

/**
 * @brief Prosty dziennik zdarzeń zapisujący do pliku (domyślnie errors.log) z datą i poziomem.
 *
 * Po init() przekierowuje też strumień błędów SFML (sf::err()) do tego samego pliku.
 */
class Logger {
public:
    /// @brief Poziom ważności komunikatu.
    enum LogLevel {INFO, WARNING, ERROR, DEBUG};
    /// @brief Otwiera plik dziennika (wywołanie wielokrotne jest bezpieczne). @param path Ścieżka pliku.
    static void init(const std::string& path = "errors.log");
    /// @brief Zapisuje komunikat. @param level Poziom. @param message Treść.
    static void log(LogLevel level, const std::string& message);
    /// @brief Zapisuje komunikat poziomu INFO. @param message Treść.
    static void info(const std::string& message){log (INFO, message);}
    /// @brief Zapisuje komunikat poziomu WARNING. @param message Treść.
    static void warning(const std::string& message){log (WARNING, message);}
    /// @brief Zapisuje komunikat poziomu ERROR. @param message Treść.
    static void error(const std::string& message){log (ERROR, message);}
    /// @brief Zapisuje komunikat poziomu DEBUG. @param message Treść.
    static void debug(const std::string& message){log (DEBUG, message);}

    /// @brief Zwraca ścieżkę pliku dziennika.
    static const std::string& path();
};


#endif //GRAFIKA2D_LOGGER_H