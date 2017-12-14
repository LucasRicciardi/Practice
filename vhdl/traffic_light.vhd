
-- Package para declaração dos semáoforos
package TrafficLightPackage is

    -- Definição do tipo estado
    type state_t is (
        GREEN_STATE,    -- Semáforo verde
        ORANGE_STATE,   -- Semáforo laranja
        RED_STATE       -- Semáforo vermelho
    );

    -- Tipo semáforo
    type TrafficLight is record
        state : state_t;          -- Estado atual deo semáforo
        active_interval : time;   -- Tempo máximo ativo
        warning_interval : time;  -- Tempo de aviso antes de mudar de estado
    end record TrafficLight;

    -- Tipo vetor de semáforos
    type TrafficLightArray is array(1 downto 0) of TrafficLight;

end package TrafficLightPackage;

-- ieee includes
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

-- debug includes
use std.textio.all;

-- 'Usa' o package
use work.TrafficLightPackage.all;

entity TrafficLightController is
    generic (
        frequency : natural;    -- frequência do clock
        tf_1_time : time;       -- Tempo ativo do semáforo 1
        tf_2_time : time;       -- Tempo ativo do semáforo 2
        w_time : time           -- Tempo de aviso antes de mudar de estado
    );
    port (
        clk : in std_logic;                              -- clock
        reset_button : in std_logic;                     -- Botão de reset
        tf_green : out std_logic_vector(1 downto 0);     -- Saída quando o semáforo n estiver verde
        tf_orange : out std_logic_vector(1 downto 0);    -- Saída quando o semáforo n estiver laranja
        tf_red : out std_logic_vector(1 downto 0)        -- Saída quando o semáforo n estiver vermelho
    );
end entity TrafficLightController;

architecture Behaviour of TrafficLightController is

-- ################################################################
-- # Signals
-- ################################################################

    -- Segundos
    signal time_interval : time := 0 sec;

    -- Handle para o semáforo ativo e para o não ativo
    signal active : integer := 0;
    signal other : integer := 1;

    -- Vetor de semáforos
    signal traffic_lights : TrafficLightArray := (
        (RED_STATE, tf_1_time, w_time),   -- Semáforo 1
        (RED_STATE, tf_2_time, w_time)    -- Semáforo 2
    );

-- ################################################################
-- # Procedimentos Auxiliares
-- ################################################################

    -- Facilita o debug
    procedure debug(msg : String; new_line : boolean) is
        variable eol : line;
    begin
        write(output, msg);
        if new_line then
            writeline(output, eol);
        end if;
    end procedure debug;

-- ################################################################
-- # Processos
-- ################################################################

begin

    -- ################################################################
    -- # Conta a quanto tempo o semáforo está ativado e controla
    -- # o estado deste com base no tempo passado
    -- ################################################################

    StateMachine : process(clk)
        variable clocks : natural := 0;             -- clocks
        variable active_interval : time := 0 sec;   -- Tempo que o semáforo está ativo
        variable warning_interval : time := 0 sec;  -- Tempo para avisar (ficar laranja)
    begin
        if rising_edge(clk) then

            -- Reinicia o sistema de precisar
            if reset_button = '1' then
                traffic_lights(0).state <= GREEN_STATE;
                active <= 0;
                traffic_lights(1).state <= RED_STATE;
                other <= 1;
                time_interval <= 0 sec;
            end if;

            -- Atualiza o contador de tempo
            clocks := clocks + 1;
            if clocks = frequency then
                time_interval <= time_interval + 1 sec;
                clocks := 0;
                debug("Tempo passado = ", false);
                debug(time'image(time_interval), true);
            end if;

            -- Recebe os valores nos 'handles' para ficar mais fácil de escrever o código
            active_interval := traffic_lights(active).active_interval;
            warning_interval := traffic_lights(active).warning_interval;

            -- Verifica se é hora de avisar sobre a mudança de estado
            if (active_interval - time_interval) = warning_interval then
                traffic_lights(active).state <= ORANGE_STATE;
            end if;

            -- Verifica se é hora de mudar de estado
            if time_interval = active_interval then
                if active = 0 then
                    active <= 1;
                    other <= 0;
                else
                    active <= 0;
                    other <= 1;
                end if;
                traffic_lights(other).state <= RED_STATE;
                traffic_lights(active).state <= GREEN_STATE;
                time_interval <= 0 sec;
                debug("Mudando de estado !", true);
            end if;
        end if;
    end process;

    -- ################################################################
    -- # Gera outputs para os semáforos com base nos estados atuais
    -- ################################################################

    UpdateOutputs : process(clk)
    begin
        if rising_edge(clk) then
            for i in traffic_lights'range loop
                case traffic_lights(i).state is
                    -- Está verde
                    when GREEN_STATE =>
                        tf_green(i) <= '1';
                        tf_orange(i) <= '0';
                        tf_red(i) <= '0';

                    -- Está laranja
                    when ORANGE_STATE =>
                        tf_green(i) <= '0';
                        tf_orange(i) <= '1';
                        tf_red(i) <= '0';

                    -- Está vermelho
                    when RED_STATE =>
                        tf_green(i) <= '0';
                        tf_orange(i) <= '0';
                        tf_red(i) <= '1';
                end case;
            end loop;
        end if;
    end process UpdateOutputs;

end architecture;
