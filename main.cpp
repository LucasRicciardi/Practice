// arduino lib includes
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Ultrasonic.h>
#include <DHT.h>

// std lib includes
#include <vector>
#include <map>
#include <functional>
#include <algorithm>

/*
*   Objetivos principais deste módulo:
*
*       1 - Controlar uma lâmpada com base no estado do sensor
*           de presença, acendendo com a ativação do sensor e
*           mantendo ela acesa durante um intervalo de tempo após
*           a última ativação do sensor => ok
*
*       2 - Ativar um buzer com intensidade variável com base
*           nos dados recebidos pelo sensor de distância => ok
*
*       3 - Telemetria dos sensores de corrente e luminosidade6
*
*       4 - Interruptor virtual de uma lâmpada => ok
*/

// #################################################################################################
// # Interface de sistema
// #################################################################################################

// Declaração da classe System Manager
class SystemManager;

class SystemInterface
{
public:
    virtual void update(SystemManager*) { }

    virtual void notify(SystemManager*, JsonObject const&) { }

    virtual void request_state(JsonObject&) { }
};

// #################################################################################################
// # Módulo gerenciador de sistemas
// #################################################################################################

class SystemManager
{
private: // Atributos privados

    // Mapa de sistemas
    std::map<String, SystemInterface*> systems;

    // Json buffer
    StaticJsonBuffer<500> json_buffer;

    // Servidor WiFi
    WiFiServer server;

    // Vetor de clintes conectados
    std::vector<WiFiClient> clients;

public: // Interface pública

    /*
    *   Construtor
    */
    SystemManager():
        systems(),
        json_buffer(),
        server(8080),
        clients()
    {}

    /*
    *   Inicialização tardia do gerenciador
    */
    SystemManager& init(const char * ssid, const char * password, const char * host)
    {
        // Conecta-se à rede WiFi
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("Not connected ...");
            delay(500);
        }

        // Seta o host
        Serial.println(
            WiFi.localIP()
        );

        // Inicia o servidor
        this->server.begin();

        return *this;
    }

    /*
    *   Adiciona um sistema ao gerenciador
    */
    SystemManager& add_system(String name, SystemInterface * sys)
    {
        this->systems.insert({ name, sys });
        return *this;
    }

    /*
    *   Retorna uma referência ao sistema de nome 'name'
    */
    template <typename SystemType>
    SystemType& get_system(String const& name)
    {
        return * (SystemType *) this->systems[name];
    }

    /*
    *   Atualiza os sistemas
    */
    void update(unsigned int dt)
    {
        // Atualiza sistemas internos
        for (auto& it : this->systems)
        {
            it.second->update(this);
        }

        // Aceita novas conexões
        WiFiClient new_client = this->server.available();
        if (new_client)
        {
            // Procura pelo IP do novo cliente na lista de clientes conectados
            bool connected = false;
            for (auto& client : this->clients)
            {
                if (client.remoteIP() == new_client.remoteIP())
                {
                    client = new_client;
                    connected = true;
                }
            }

            // Adiciona o novo cliente se ele ainda não está conectado
            if (not connected)
            {
                this->clients.push_back(new_client);
            }
        }

        // Delay
        delay(dt);

        // Processa pedidos de clientes conectados
        for (auto& client : this->clients)
        {
            if (client.available())
            {
                // Lê o pedido
                String message = client.readStringUntil('\n');
                JsonObject const& request = this->json_buffer.parseObject(message);

                // Verifica se é uma solicitação
                String addressee = request.get<String>("addressee");
                if (addressee == "request_state")
                {
                    JsonObject& response = this->json_buffer.createObject();
                    for (auto& it : this->systems)
                    {
                        JsonObject& state = response.createNestedObject(it.first);
                        it.second->request_state(state);
                    }
                    message = "";
                    response.printTo(message);
                    client.println(message);
                    client.flush();
                }

                // Ou se é um comando
                else
                {
                    this->send_message(addressee, [&] (JsonObject& message)
                    {
                        for (auto& obj : request)
                        {
                            message.set(obj.key, obj.value);
                        }
                    });
                }

                // Limpa o buffer
                this->json_buffer.clear();
            }
        }

        // Remove clientes desconectados
        std::remove_if(this->clients.begin(), this->clients.end(), [&] (WiFiClient& client)
        {
            return not client.connected();
        });

        // Delay
        delay(dt);
    }

    /*
    *   Envia uma mensagem a todos os sistemas
    */
    void send_message(String const& addressee, std::function<void(JsonObject&)> add_content)
    {
        // Cria um objeto para mensagem
        JsonObject& message = this->json_buffer.createObject();

        // Adiciona conteúdo à mensagem
        add_content(message);

        // Despacha a mensagem para todos os sistemas
        if (addressee == "all")
        {
            for (auto& it : this->systems)
            {
                it.second->notify(this, message);
            }
        }

        // Despacha somente para um destinatário
        else
        {
            auto it = this->systems.find(addressee);
            if (it != this->systems.end())
            {
                this->systems[addressee]->notify(this, message);
            }
        }

        // Limpa o buffer
        this->json_buffer.clear();
    }
};

// #################################################################################################
// # Sistema Controlador de Output
// #################################################################################################

class OutputControlSystem:
    public SystemInterface
{
private:

    // Estrutura pino
    struct Pin
    {
        int number;
        bool state;

        Pin(int p, bool s):
            number(p),
            state(s)
        {}
    };

    // Vetor de pinos
    std::vector<Pin> pins;

public: // Interface pública

    /*
    *   Construtor da classe
    */
    OutputControlSystem(std::vector<int> pins):
        pins()
    {
        for (auto& pin : pins)
        {
            pinMode(pin, OUTPUT);
            this->pins.push_back(
                Pin(pin, LOW)
            );
        }
    }

    /*
    *   Recebe notificações do servidor
    */
    void notify(SystemManager * sys, JsonObject const& message)
    {
        // Veririca se a mensagem contém os parâmetros
        // que interessa ao módulo
        if (message.containsKey("pin") and message.containsKey("state"))
        {
            // Recebe a mensagem
            auto pin_number = message.get<int>("pin");
            auto state = message.get<bool>("state");

            // Atualiza o estado da lâmpada
            for (auto& pin : this->pins)
            {
                // Debug
                Serial.print("\n\nPino => ");
                Serial.print(pin.number);
                Serial.print(" Estado => ");
                Serial.println(pin.state);

                if (pin.number == pin_number)
                {
                    pin.state = state;
                    break;
                }
            }
        }
    }

    /*
    *   Atualiza o estado do controlador
    */
    void update(SystemManager *)
    {
        for (auto pin : this->pins)
        {
            digitalWrite(pin.number, pin.state);
        }
    }

    /*
    *   Retorna o estado do sistema
    */
    void request_state(JsonObject& state)
    {
        for (auto& pin : this->pins)
        {
            state.set(
                String(pin.number),
                String(pin.state)
            );
        }
    }
};

// #################################################################################################
// # Sistema Controlador do Sensor de Presença
// #################################################################################################

class OccupancySensorControlSystem:
    public SystemInterface
{
private: // Atributos privados

    // Domínio do problema
    using Time = unsigned long;

    // Vetor com os pinos em que este sensor opera
    std::vector<int> output_pins;

    // Pino em que o sensor está ligado
    int pir;

    // Ponto de ativação no domínio do tempo
    Time activation_point;

    // Intervalo de tempo em ms que o sensor fica ativo
    Time activation_interval;

public: // Interface publica

    /*
    *   Construtor
    */
    OccupancySensorControlSystem(int p, Time dt, std::vector<int> o):
        pir(p),
        output_pins(o),
        activation_point(0),
        activation_interval(dt)
    {
        pinMode(this->pir, INPUT);
    }

    bool read_sensor(int led_pin)
    {
        return digitalRead(led_pin);
    }

    /*
    *   Ativa o pino correspondente até que dt tenha passado
    */
    void update(SystemManager * sys)
    {
        #define ON HIGH
        #define OFF LOW

        // Recebe o estado atual do sensor
        bool sensor_state = this->read_sensor(this->pir);

        // Debug
        Serial.print("\n\nEstado do sensor de Presença => ");
        Serial.println(sensor_state);

        // Se o sensor de presença estiver ativado
        if (sensor_state == ON)
        {
            // Envia uma mensagem aos pinos associados
            for (auto& pin : this->output_pins)
            {
                sys->send_message("output_control", [&] (JsonObject& message)
                {
                    message.set("pin", pin);
                    message.set("state", LOW);
                });
            }

            // Atualiza o tempo em que o sensor foi ativado
            this->activation_point = millis();
        }

        // Se não
        else if (sensor_state == OFF)
        {
            // Verifica o intervalo desde o último ponto de ativação é maior do que dt
            if (millis() - this->activation_point > this->activation_interval)
            {
                // Notifica os pinos para apagarem
                for (auto& pin : this->output_pins)
                {
                    sys->send_message("output_control", [&] (JsonObject& message)
                    {
                        message.set("pin", pin);
                        message.set("state", HIGH);
                    });
                }
            }
        }

        #undef ON
        #undef OFF
    }
};

// #################################################################################################
// # Sistema Controlador do Sensor Ultrassônico
// #################################################################################################

class UltrasonicSensorControlSystem:
    public SystemInterface
{
private: // Atributos privados

    // Par de pinos para o sensor ultrassônico
    std::pair<int, int> sensor_pin;

    // Pino de output
    int output_pin;

    // Valor mínimo de ativação (threshold)
    int threshold;

    // Tempo do último pulso
    unsigned long last_pulse;

    // Sensor ultrassônico
    Ultrasonic ultrasonic;

public: // Interface pública

    /*
    *   Construtor
    */
    UltrasonicSensorControlSystem(std::pair<int, int> s, int o, int t):
        sensor_pin(s),
        output_pin(o),
        threshold(t),
        last_pulse(0),
        ultrasonic(s.first, s.second)
    {
    }

    /*
    *   Lê o sensor de presença
    */
    double read_sensor()
    {
        // Lê o sensor
        long microsec = ultrasonic.timing();
        double value = ultrasonic.convert(microsec, Ultrasonic::CM);

        // Debug
        Serial.print("Distância => ");
        Serial.print(value);
        Serial.println("m");

        // Retorna o valor
        return value;
    }

    /*
    *   Manda uma frequência no pino de saída de o valor
    *   do sensor for menor do que o valor mínimo (threshold)
    */
    void update(SystemManager * sys)
    {
        #define ON false
        #define OFF true

        // Recebe o valor do pino do sensor ultrasônico
        double value = this->read_sensor();

        // Se distância é maior que a distância mínima (threshold), liga o bip
        if (value <= this->threshold)
        {
            // Quanto mais próximo de zero, a partir de threshold,
            // maior será a frequência do bip
            unsigned long interval = 1000 * (value / this->threshold);
            if (millis() - this->last_pulse > interval)
            {
                sys->send_message("output_control", [&] (JsonObject& message) -> void
                {
                    message.set("pin", this->output_pin);
                    message.set(
                        "state", !digitalRead(this->output_pin)
                    );
                });
                this->last_pulse = millis();
            }
        }

        // Se não, desliga o bip
        else digitalWrite(
            this->output_pin, LOW
        );

        #undef ON
        #undef OFF
    }
};

// #################################################################################################
// # Sistema Controlador de Telemetria
// #################################################################################################

class TelemetryControlSystem:
    public SystemInterface
{
private: // Atributos privados

    // Sensor de humidade e temperatura
    DHT dht;

    // Valores dos sensores
    using SensorValue = double;
    SensorValue humidity;
    SensorValue temperature;
    SensorValue luminosity;

public: // Interface pública

    TelemetryControlSystem(int dht_pin):
        dht(dht_pin, DHT11),
        humidity(0),
        temperature(0),
        luminosity(0)
    {
        this->dht.begin();
    }

    /*
    *   Atualiza os valores dos sensores
    */
    void update(SystemManager*)
    {
        // Humidade, temperatura e luminosidade
        this->humidity = this->dht.readHumidity();
        this->temperature = this->dht.readTemperature();
        this->luminosity = analogRead(A0);

        // Debug
        Serial.print("\n\nHumidade => ");
        Serial.println(this->humidity);

        Serial.print("Temperatura => ");
        Serial.println(this->temperature);

        Serial.print("Luminosidade => ");
        Serial.println(this->luminosity);
    }

    /*
    *    Retorna os valores dos sensores !!
    */
    void request_state(JsonObject& state)
    {
        /// Envia os valores
        state.set("humidity", std::isnan(this->humidity) ? 0 : this->humidity);
        state.set("temperature", std::isnan(this->temperature) ? 26 : this->temperature);
        state.set("luminosity", this->luminosity);
        state.set("current", random(0, 15));
    }
};

// #################################################################################################
// # Modus Operandi
// #################################################################################################

// Gerenciador de sistemas
SystemManager system_manager;

void setup()
{
    // Serial para debug
    Serial.begin(9600);
    delay(500);

    // Parâmetros da rede WiFi

    /*
    const char * ssid = "TP-LINK_FB88";
    const char * password = "Da5ch35#1#2";
    */

    const char * ssid = "INTERNET";
    const char * password = "";

    const char * host = "Arduino_1";

    // Adiciona sistemas ao gerenciador e inicia os sistemas
    system_manager

        // Inicia o sistema, paramêtros:
        //      const char * ssid =>
        //          Nome da rede WiFi
        //
        //      const char * password =>
        //          Senha da rede WiFi
        //
        //      const char * host =>
        //          Nome do Dispositivo na Rede

        .init(ssid, password, host)

        // Sistema controlador de output, paramêtros:
        //      vector<int> v =>
        //          Vetor de inteiros conténdo os pinos que
        //          serão controlados por este sistema

        .add_system(
            "output_control",
            new OutputControlSystem(
                { D5, D7, D2 }
            )
        )

        // Sistema do sensor de presença, paramêtros:
        //      int n =>
        //          Inteiro contendo o número do pino onde o sensor está ligado
        //
        //      int t =>
        //          Intervalo de tempo, em ms, que o pino ficará ativo após o disparo do sensor
        //
        //      vector<int> v =>
        //          Vetor de inteiros contendo os pinos
        //          que responderão à ativação deste sensor

        .add_system(
            "pir_control",
            new OccupancySensorControlSystem(
                D6, 5 * 1000, { D7 }
            )
        )


        // Sistema do sensor ultrassônico, paramêtros:
        //      pair<int, int> p =>
        //          Par de inteiros conténdo os pinos onde
        //          o sensor de ultra estará ligado
        //
        //      int n =>
        //          Pino que emitirá um pulso com uma frequência
        //          variável de acordo com o valor de 'threshold'
        //
        //      int t =>
        //          Valor mínimo para que módulo comece a disparar
        //          pulsos, a frequência de emissão é dada por:
        //          f = 1000 * (v/t), onde v é valor lido
        //          do sensor, o sensor só irá disparar um pulso
        //          quando v <= t


        .add_system(
            "distance_control",
            new UltrasonicSensorControlSystem(
                { D3, D4 }, D5, 10
            )
        )

        // Sistema de controle de telemetria, paramêtros:
        //      int n =>
        //          Pino do sensor dht

        .add_system(
            "telemetry_control",
            new TelemetryControlSystem(D8)
        )
    ;
}

// Atualiza o estado do dispositivo
void loop()
{
    // Atualiza os sistemas
    const int delay = 50;
    system_manager.update(delay);
}
