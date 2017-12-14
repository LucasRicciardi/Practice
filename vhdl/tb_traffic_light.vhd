library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

-- debug inclues
use std.textio.all;

entity TrafficLightTb is
end entity;

architecture behaviour of TrafficLightTb is

    -- componente
    component TrafficLightController
    generic (
        frequency : natural;    -- frequência do clock

        tf_1_time : time;       -- Tempo ativo do semáforo 1
        tf_2_time : time;       -- Tempo ativo do semáforo 2
        w_time : time           -- Tempo ativo do semáforo 3
    );
    port (
        clk : in std_logic;             -- clock
        reset_button : in std_logic;    -- botão de reiniciar

        tf_1_green : out std_logic;     -- Saída quando o semáforo 1 estiver verde
        tf_1_orange : out std_logic;    -- Saída quando o semáforo 1 estiver laranja
        tf_1_red : out std_logic;       -- Saída quando o semáforo 1 estiver vermelho

        tf_2_green : out std_logic;     -- Saída quando o semáforo 2 estiver verde
        tf_2_orange : out std_logic;    -- Saída quando o semáforo 2 estiver laranja
        tf_2_red : out std_logic        -- Saída quando o semáforo 2 estiver vermelho
    );
    end component TrafficLightController;

    -- clock
    signal clock : std_logic := '0';
    signal reset_button : std_logic := '0';

    -- Saídas dos semáforos
    signal tf_1_green : std_logic := '0';
    signal tf_1_orange : std_logic := '0';
    signal tf_1_red : std_logic := '0';
    signal tf_2_green : std_logic := '0';
    signal tf_2_orange : std_logic := '0';
    signal tf_2_red : std_logic := '0';

    -- Saídas dos semáforos de pedestre
    signal pedestrian_tf_1_green : std_logic := '0';
    signal pedestrian_tf_1_orange : std_logic := '0';
    signal pedestrian_tf_1_red : std_logic := '0';
    signal pedestrian_tf_2_green : std_logic := '0';
    signal pedestrian_tf_2_orange : std_logic := '0';
    signal pedestrian_tf_2_red : std_logic := '0';

begin
    -- Instância do controlador de semáforos
    traffic_light_controller : entity work.TrafficLightController
        generic map(
            frequency => 50 * 1000,             -- clocks/segundo
            tf_1_time => 18 sec,                -- Tempo ativo do semáforo 1
            tf_2_time => 20 sec,                -- Tempo ativo do semáforo 2
            w_time => 2 sec                     -- Tempo antes de mudar de estado para avisar
        )
        port map(
            -- Clock e botão de reset
            clk => clock,
            reset_button => reset_button,

            -- Saídas dos semáforo 1 e 2
            tf_green(0) => tf_1_green,
            tf_green(1) => tf_2_green,

            tf_orange(0) => tf_1_orange,
            tf_orange(1) => tf_2_orange,

            tf_red(0) => tf_1_red,
            tf_red(1) => tf_2_red
        );

    -- Instância do controlador de semáforos de pedestres
    pedestrian_traffic_light_controller : entity work.TrafficLightController
        generic map(
            frequency => 50 * 1000,             -- clocks/segundo
            tf_1_time => 18 sec,                -- Tempo ativo do semáforo 1
            tf_2_time => 20 sec,                -- Tempo ativo do semáforo 2
            w_time => 2 sec                     -- Tempo antes de mudar de estado para avisar
        )
        port map(
            -- Clock e botão de reset
            clk => clock,
            reset_button => reset_button,

            -- Saídas dos semáforo 1 e 2
            tf_green(0) => pedestrian_tf_1_green,
            tf_green(1) => pedestrian_tf_2_green,

            tf_orange(0) => pedestrian_tf_1_orange,
            tf_orange(1) => pedestrian_tf_2_orange,

            tf_red(0) => pedestrian_tf_1_red,
            tf_red(1) => pedestrian_tf_2_red
        );

    -- Gerador de clocks
    ClockGenerator : process
    begin
        -- Inicia os sistemas
        reset_button <= '1';
        wait for 10 ms;
        reset_button <= '0';

        -- Entra no loop
        while true loop
            clock <= '1';
            wait for 0.01 ms;
            clock <= '0';
            wait for 0.01 ms;
        end loop;
        wait;
    end process;

end architecture;
