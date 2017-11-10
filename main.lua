
function main()

    -- Espaço em função do tempo
    function s(s0, v)
        -- Variáveis
        local s0 = s0 or 0
        local v = v or 1

        -- Função
        return function(t)
            return s0 + v*t
        end
    end

    -- Cria uma equação
    f = s(10p, 5)
    for i = 0, 100, 1 do
        print(i, f(i))
    end
end

main()
