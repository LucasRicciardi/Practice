#include <stdio.h>

typedef struct context
{
    int init;
    int loop;

} Context;

void state_machine(int context)
{
    switch (context)
    {
        case 10:
        {
            // Inicia a máquina
            printf("%s\n\n", "Initializing machine 1");
            return;

            // Entra no loop da máquina
            int loop = 0;
            while (1)
            {
                printf("%s %d\n\n", "Running Machine 1", loop);
                if (loop++ > 5)
                {
                    loop = (loop + 1) % 10;
                    return;
                }
                case 11:;
            }
        } break;

        case 20:
        {
            // Inicia a máquina
            printf("%s\n\n", "Initializing machine 2");
            return;

            // Entra no loop da máquina
            int loop = 0;
            while (1)
            {
                printf("%s Loop: %d\n\n", "       Running Machine 2", loop);
                if (loop++ > 0)
                {
                    loop = (loop + 1) % 10;
                    return;
                }
                case 21:;
            }
        } break;
    }
}

int main()
{
    Context m1 = { 10, 11 };
    Context m2 = { 20, 21 };
    state_machine(m1.init);
    state_machine(m2.init);

    while (1)
    {
        state_machine(m1.loop);
        state_machine(m2.loop);
    }
    return 0;
}
