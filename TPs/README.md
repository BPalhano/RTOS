Vamos resolver cada parte da prova passo a passo, seguindo as especificações do enunciado.

---

### **1. Configuração do Projeto STM32 (10 min)**
**Passos:**
1. Abra o STM32CubeIDE e crie um novo projeto para a placa **STM32L073RZ** ou **STM32L476RG**.
2. Nomeie o projeto como `eval_tos_SEUNOME`.
3. Em **Project Manager > Advanced Settings**, desative o **USART2**.
4. Configure o FreeRTOS:
   - Selecione **CMSIS_V1**.
   - Em **Config Parameters**:
     - `Memory Allocation`: **Dynamic**.
     - `USE_TASK_NOTIFICATIONS`: **Enabled** (os demais campos como **Disabled**).
   - Em **Include Parameters**, desative todos os recursos.
5. Salve e gere o código. Compile o projeto para garantir que não há erros/warnings.

---

### **2. Drivers UART (10 min)**
**Passos:**
1. Adicione os arquivos `ensi_uart.c` e `ensi_uart.h` ao seu workspace (via drag-and-drop).
2. No arquivo `ensi_uart.h`, descomente a linha correspondente à sua placa (ex: `#define NUCLEO_L073RZ`).
3. Inicialize o UART no `main()`:
   ```c
   MX_USART2_UART_Init(); // Substitua pelo nome correto da função gerada pelo CubeMX
   ```
4. Envie a mensagem de inicialização após a inicialização dos periféricos:
   ```c
   printf("\r\n*** APPLICATION STARTING SEUNOME ***\r\n");
   ```
5. Compile e teste no terminal serial (9600 bps). Valide com o professor.

---

### **3. Implementação da Aplicação Principal (Diagrama)**
**Estrutura das Tarefas:**
- **Tarefas:**
  - `lecture_uart` (Prioridade 1)
  - `afficher_minuteur` (Prioridade 2)
  - `lecture_bouton_poussoir` (Prioridade 1)
  - `decompteur` (Prioridade 1)

**Código Base (em `Freertos.c`):**
```c
// Variáveis globais
volatile int32_t minuteur = 0;
volatile bool decompte_actif = false;
SemaphoreHandle_t sema_afficher_minuteur;
QueueHandle_t queue_decompte;

// Tarefa: lecture_uart
void lecture_uart_task(void *pvParameters) {
    char c;
    while (1) {
        c = uart_receive(); // Função do driver UART
        switch (c) {
            case 'U': minuteur += 1; break;
            case 'D': minuteur += 10; break;
            case 'C': minuteur += 100; break;
            case 'u': minuteur -= 1; break;
            case 'd': minuteur -= 10; break;
            case 'c': minuteur -= 100; break;
        }
        xSemaphoreGive(sema_afficher_minuteur); // Notifica a atualização
    }
}

// Tarefa: afficher_minuteur
void afficher_minuteur_task(void *pvParameters) {
    while (1) {
        xSemaphoreTake(sema_afficher_minuteur, portMAX_DELAY);
        printf("Minuteur: %ld\r\n", minuteur);
    }
}

// Tarefa: lecture_bouton_poussoir (com anti-rebond)
void lecture_bouton_task(void *pvParameters) {
    while (1) {
        if (HAL_GPIO_ReadPin(Bouton_GPIO_Port, Bouton_Pin) == GPIO_PIN_SET) {
            vTaskDelay(pdMS_TO_TICKS(50)); // Anti-rebond
            decompte_actif = !decompte_actif;
            xQueueSend(queue_decompte, &decompte_actif, 0); // Para o exercício adicional
            while (HAL_GPIO_ReadPin(Bouton_GPIO_Port, Bouton_Pin) == GPIO_PIN_SET) {
                vTaskDelay(1);
            }
        }
        vTaskDelay(1);
    }
}

// Tarefa: decompteur
void decompteur_task(void *pvParameters) {
    bool decompte_actif_local = false;
    while (1) {
        xQueueReceive(queue_decompte, &decompte_actif_local, portMAX_DELAY);
        if (decompte_actif_local && minuteur > 0) {
            minuteur--;
            xSemaphoreGive(sema_afficher_minuteur);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

// main.c
void MX_FREERTOS_Init(void) {
    sema_afficher_minuteur = xSemaphoreCreateBinary();
    queue_decompte = xQueueCreate(1, sizeof(bool));
    xTaskCreate(lecture_uart_task, "UART", 128, NULL, 1, NULL);
    xTaskCreate(afficher_minuteur_task, "Afficheur", 128, NULL, 2, NULL);
    xTaskCreate(lecture_bouton_task, "Bouton", 128, NULL, 1, NULL);
    xTaskCreate(decompteur_task, "Decompteur", 128, NULL, 1, NULL);
}
```

---

### **4. Exercícios Adicionais**
#### **a) Detecção de Debordamento de Memória (5-10 min)**
- Adicione no `FreeRTOSConfig.h`:
  ```c
  #define configCHECK_FOR_STACK_OVERFLOW 2
  #define configUSE_MALLOC_FAILED_HOOK 1
  ```
- Implemente os hooks:
  ```c
  void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
      printf("Stack overflow na tarefa: %s\r\n", pcTaskName);
  }

  void vApplicationMallocFailedHook(void) {
      printf("Falha na alocacao de memoria!\r\n");
  }
  ```

#### **b) Proteção da Variável `minuteur` (15 min)**
- Crie um mutex:
  ```c
  SemaphoreHandle_t mutex_minuteur = xSemaphoreCreateMutex();
  ```
- Modifique o acesso à variável:
  ```c
  xSemaphoreTake(mutex_minuteur, portMAX_DELAY);
  minuteur += 1; // Exemplo
  xSemaphoreGive(mutex_minuteur);
  ```

#### **c) Substituição de `decompte_actif` por Queue (15 min)**
- Remova a variável global `decompte_actif`.
- Use a fila `queue_decompte` (já implementada no código acima).

#### **d) Nova Tarefa `minuteur_a_zero` (15 min)**
```c
SemaphoreHandle_t sema_zero = xSemaphoreCreateBinary();

void minuteur_a_zero_task(void *pvParameters) {
    while (1) {
        xSemaphoreTake(sema_zero, portMAX_DELAY);
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
        // Aguarda entrada do usuário para desligar
        char c = uart_receive();
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    }
}

// Na tarefa decompteur:
if (minuteur == 0) {
    xSemaphoreGive(sema_zero);
}
```

---

### **Validação Final**
- Compile e teste cada funcionalidade separadamente.
- Chame o professor para validar cada etapa conforme as instruções.
