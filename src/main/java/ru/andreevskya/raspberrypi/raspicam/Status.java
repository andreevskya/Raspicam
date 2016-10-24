package ru.andreevskya.raspberrypi.raspicam;

/**
 * Перечисление кодов возврата из API.
 * См. http://www.jvcref.com/files/PI/documentation/html/group___mmal_types.html#ga5ac87230bd615d2b154a9d5899bc37fd
 */
public enum Status {
    /**
     * Успешно.
     */
    SUCCESS,
    /**
     * Недостаточно памяти.
     */
    ENOMEM,
    /**
     * Недостаточно ресурсов (отличных от памяти).
     */
    ENOSPC,
    /**
     * Некорректный аргумент.
     */
    EINVAL,
    /**
     * Вызываемый метод не реализован.
     */
    ENOSYS,
    /**
     * Нет такого файла или директрии.
     */
    ENOENT,
    /**
     * Нет такого устройства или адреса.
     */
    ENXIO,
    /**
     * Ошибка ввода/вывода.
     */
    EIO,
    /**
     * Некорректный поиск.
     */
    ESPIPE,
    /**
     * Данные повреждены.
     */
    ECORRUPT,
    /**
     * Компонент не готов.
     */
    ENOTREADY,
    /**
     * Компонент не сконфигурирован.
     */
    ECONFIG,
    /**
     * Порт уже подключен.
     */
    EISCONN,
    /**
     * Порт не подключен.
     */
    ENOTCONN,
    /**
     * Ресурс временно недоступен.
     */
    EAGAIN,
    /**
     * Некорректный адрес.
     */
    EFAULT,
    /**
     * Force to 32 bit (ЩИТО?)
     */
    STATUS_MAX,
    /**
     * Неизвестный статус. Означает, что текущей реализации незнаком статус, вернувшийся из нативной либы.
     */
    UNKNOWN
}
