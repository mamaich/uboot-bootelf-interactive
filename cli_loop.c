void _start(void) {
    // Извлекаем значение регистра LR (x30) с помощью встроенного ассемблера
    unsigned long long start_addr;
    __asm__ volatile (
        "mov %0, x30"
        : "=r" (start_addr)
        :
        : 
    );

    // Выравнивание адреса на границу 16 МБ (0x1000000)
    start_addr &= ~(0x1000000ULL - 1);

    // Шаблон для поиска: FD 7B BF A9 FD 03 00 91 ? ? ? 97 00 00 00 14
    // Пропускаемые байты обозначены как -1
    int pattern[] = {0xFD, 0x7B, 0xBF, 0xA9, 0xFD, 0x03, 0x00, 0x91, -1, -1, -1, 0x97, 0x00, 0x00, 0x00, 0x14};
    unsigned int pattern_len = 16;

    unsigned char *current = (unsigned char *)start_addr;
    unsigned int i;

    // Поиск шаблона
    while (1) {
        unsigned int match = 1;
        
        // Проверка совпадения байтов с учетом пропускаемых позиций (-1)
        for (i = 0; i < pattern_len; i++) {
            if (pattern[i] != -1 && current[i] != (unsigned char)pattern[i]) {
                match = 0;
                break;
            }
        }

        // Если шаблон найден, вызываем функцию
        if (match) {
            void (*func)() = (void (*)())current;
            func();
            return;
        }

        // Сдвигаемся на следующий байт
        current++;

        // Проверка на выход за пределы памяти (опционально, зависит от системы)
        // Здесь предполагается, что поиск ограничен архитектурой/контекстом
    }
}