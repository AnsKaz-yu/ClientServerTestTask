Формулировка задания находится в TASK.md.

## Как запустить проект

Сначала соберите проект:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
````

Потом запустите сервер:

```bash
./build/server/DeviceMonitorServer
```

В открывшемся окне нажмите `Start Server`.

После этого можно запустить клиент:

```bash
./build/client/DeviceMonitorClient
```

Если сервер запущен не на локальной машине, укажите адрес вручную:

```bash
./build/client/DeviceMonitorClient --host 127.0.0.1 --port 12345
```

Когда клиенты подключатся, в интерфейсе сервера нажмите `Start All Clients` — после этого начнётся передача данных.

Для остановки:

`Stop All Clients` — остановить отправку данных; 
`Stop Server` — полностью выключить сервер


