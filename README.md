# cpp-transport-catalogue
Финальный проект: транспортный справочник
TransportCatalogue 
Реализация транспортного справочника. 
Программа умеет строить карту маршрутов и получаеть данные по маршрутам и остановкам на её основе.
Получение и сохранение данных в формате JSON с использованием собственной библиотеки.
Визуализация карты маршрутов в формате SVG с использованием собственной библиотеки.
Поиск кратчайшего пути по заданным условиям на основе построенной карты маршрутов.
Сериализация и десериализация транспортного каталока с использованием Google Protobuf.


## Работа с транспортным каталогом
Класс **TransportCatalogue** поддерживает следующие основные методы:
```c++
 // добавляет остановку в каталог
void AddStop(const std::string &stop_name, geo::Coordinates coordinate);
// формирует маршрут из списка остановок и добавляет его в каталог.
// все остановки по маршруту должны быть предварительно добавлены методом AddStop
// если какой-то остановки из списка нет в каталоге - выбрасывает исключение
void AddRoute(const std::string &route_name, domain::RouteType route_type, const std::vector<std::string> &stops);
// добавляет в каталог информацию о расстоянии между двумя остановками
// если какой-то из остановок нет в каталоге - выбрасывает исключение
void SetDistance(const std::string &stop_from, const std::string &stop_to, int distance);
// возвращает информацию о маршруе по его имени
// если маршрута нет в каталоге - выбрасывает исключение std::out_of_range
domain::RouteInfo GetRouteInfo(const std::string &route_name) const;
// возвращает список автобусов, проходящих через остановку
// если остановки нет в каталоге - выбрасывает исключение std::out_of_range
std::optional<std::reference_wrapper<const std::set<std::string_view>>>
GetBusesOnStop(const std::string &stop_name) const;
// возвращает расстояние между остановками 1 и 2 - в прямом, либо если нет - в обратном направлении
// если информации о расстоянии нет в каталоге - выбрасывает исключение
int GetDistance(const std::string &stop_from, const std::string &stop_to) const;
```
Для реализации основных возможностей каталога реализован "фасад" **TransportCatalogueHandler**. Основные методы фасада:
```c++
// конструктор, принимает транспортный каталог по ссылке
explicit TransportCatalogueHandler(TransportCatalogue &catalogue);
// возвращает информацию о маршруе по его имени
// если маршрута нет в каталоге - выбрасывает исключение std::out_of_range
domain::RouteInfo GetRouteInfo(const std::string &route_name) const;
// возвращает список автобусов, проходящих через остановку
// если остановки нет в каталоге - выбрасывает исключение std::out_of_range
std::optional<std::reference_wrapper<const std::set<std::string_view>>>
GetBusesOnStop(const std::string &stop_name) const;
// возвращает сформированную "карту" маршрутов в формате svg-документа
svg::Document RenderMap() const;
// возвращает маршрут между двумя остановками
std::optional<Route> BuildRoute(const std::string &from, const std::string &to);
// загружает все доступные данные из JSON
void LoadDataFromJson(const json_reader::JsonIO &json);
// загружает запросы из Json и выводит ответы в поток out
void LoadRequestsAndAnswer(const json_reader::JsonIO &json, std::ostream &out);
// Сериализует доступные данные
bool SerializeData();
// Десериализует доступные данные
bool DeserializeData();
```

## Сборка с помощью CMake
> 0. Скачайте и соберите Google Protobuf под вашу версию компилятора
> 1. Создайте папку для сборки программы
> 2. Откройте консоль в данной папке и введите в консоли : `cmake <путь к файлу CMakeLists.txt> -DCMAKE_PREFIX_PATH= <путь к собранной библиотеке Protobuf>` 
> 3. Чтобы отключить сборку тестов добавьте к предыдущей команде ключ : `-DTESTING = OFF`
> 4. Введите команду : `cmake --build .` 
> 5. После сборки в папке сборки появится исполняемый файл `transport_catalogue.exe`.
## Использование собранной версии программы

Для работы программы в папке с программой надо предварительно создать файлы `make_base.json` и `process_requests.json`\
\
Файл `make_base.json` должен представлять собой словарь JSON со следующими разделами (ключами) :\
`serialization_settings` - настройки сериализации.\
`routing_settings` - настройки маршрутизации. \
`render_settings` - настройки отрисовки. \
`base_requests` - массив данных об остановках и маршрутах\
<details>
  <summary>Пример корректного файла make_base.json:</summary>

```json 
  {
      "serialization_settings": {
          "file": "transport_catalogue.db"
      },
      "routing_settings": {
          "bus_wait_time": 2,
          "bus_velocity": 30
      },
      "render_settings": {
          "width": 1200,
          "height": 500,
          "padding": 50,
          "stop_radius": 5,
          "line_width": 14,
          "bus_label_font_size": 20,
          "bus_label_offset": [
              7,
              15
          ],
          "stop_label_font_size": 18,
          "stop_label_offset": [
              7,
              -3
          ],
          "underlayer_color": [
              255,
              255,
              255,
              0.85
          ],
          "underlayer_width": 3,
          "color_palette": [
              "green",
              [
                  255,
                  160,
                  0
              ],
              "red"
          ]
      },
      "base_requests": [
          {
              "type": "Bus",
              "name": "14",
              "stops": [
                  "Улица Лизы Чайкиной",
                  "Электросети",
                  "Ривьерский мост",
                  "Гостиница Сочи",
                  "Кубанская улица",
                  "По требованию",
                  "Улица Докучаева",
                  "Улица Лизы Чайкиной"
              ],
              "is_roundtrip": true
          },
          {
              "type": "Bus",
              "name": "24",
              "stops": [
                  "Улица Докучаева",
                  "Параллельная улица",
                  "Электросети",
                  "Санаторий Родина"
              ],
              "is_roundtrip": false
          },
          {
              "type": "Bus",
              "name": "114",
              "stops": [
                  "Морской вокзал",
                  "Ривьерский мост"
              ],
              "is_roundtrip": false
          },
          {
              "type": "Stop",
              "name": "Улица Лизы Чайкиной",
              "latitude": 43.590317,
              "longitude": 39.746833,
              "road_distances": {
                  "Электросети": 4300,
                  "Улица Докучаева": 2000
              }
          },
          {
              "type": "Stop",
              "name": "Морской вокзал",
              "latitude": 43.581969,
              "longitude": 39.719848,
              "road_distances": {
                  "Ривьерский мост": 850
              }
          },
          {
              "type": "Stop",
              "name": "Электросети",
              "latitude": 43.598701,
              "longitude": 39.730623,
              "road_distances": {
                  "Санаторий Родина": 4500,
                  "Параллельная улица": 1200,
                  "Ривьерский мост": 1900
              }
          },
          {
              "type": "Stop",
              "name": "Ривьерский мост",
              "latitude": 43.587795,
              "longitude": 39.716901,
              "road_distances": {
                  "Морской вокзал": 850,
                  "Гостиница Сочи": 1740
              }
          },
          {
              "type": "Stop",
              "name": "Гостиница Сочи",
              "latitude": 43.578079,
              "longitude": 39.728068,
              "road_distances": {
                  "Кубанская улица": 320
              }
          },
          {
              "type": "Stop",
              "name": "Кубанская улица",
              "latitude": 43.578509,
              "longitude": 39.730959,
              "road_distances": {
                  "По требованию": 370
              }
          },
          {
              "type": "Stop",
              "name": "По требованию",
              "latitude": 43.579285,
              "longitude": 39.733742,
              "road_distances": {
                  "Улица Докучаева": 600
              }
          },
          {
              "type": "Stop",
              "name": "Улица Докучаева",
              "latitude": 43.585586,
              "longitude": 39.733879,
              "road_distances": {
                  "Параллельная улица": 1100
              }
          },
          {
              "type": "Stop",
              "name": "Параллельная улица",
              "latitude": 43.590041,
              "longitude": 39.732886,
              "road_distances": {}
          },
          {
              "type": "Stop",
              "name": "Санаторий Родина",
              "latitude": 43.601202,
              "longitude": 39.715498,
              "road_distances": {}
          }
      ]
  }  
```
</details>

\
Файл `process_requests.json` должен представлять собой словарь JSON со следующими разделами (ключами) :\
`serialization_settings` - настройки сериализации.\
`stat_requests` - массив запросов к каталогу

<details>
  <summary>Пример корректного файла process_requests.json:</summary>

```json 
  {
      "serialization_settings": {
          "file": "transport_catalogue.db"
      },
      "stat_requests": [
          {
              "id": 218563507,
              "type": "Bus",
              "name": "14"
          },
          {
              "id": 508658276,
              "type": "Stop",
              "name": "Электросети"
          },
          {
              "id": 1964680131,
              "type": "Route",
              "from": "Морской вокзал",
              "to": "Параллельная улица"
          },
          {
              "id": 1359372752,
              "type": "Map"
          }
      ]
  }
```
</details>

\
После этого можно приступать к работе с программой.
### Формирование транспортного каталога
Запустите собранную программу с ключом : `./transport_catalogue make_base`\
Программа прочитает файл `make_base.json` и сформирует на его основе транспортный каталог.
В папке с программой появится файл `transport_catalogue.db` (или другой, в зависимости от того, какое название будет указано в `"serialization_settings"`). В данном файле будет сохранен каталог в двоичном виде.\
В дальнейшем этот сохраненный каталог можно будет "разворачивать" для формирования ответов на запросы, без необходимости строить его заново.

### Использование сформированного транспортного каталога
Запустите собранную программу с ключом : `./transport_catalogue process_requests`\
Программа прочитает файл `process_requests.json`. В данном файле в настройках `"serialization_settings"` должно быть указано имя существующего файла с двоичным представлением сформированного транспортного каталога.
После "развертывания" каталога из двоичного файла, программа последовательно обойдет запросы из `"stat_requests"` и сохранит сформированные ответы в файл `result.json`
