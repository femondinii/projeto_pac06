# Projeto IA + IoT previsão de chuva

Estudantes: Guilherme Bessa, Gustavo Larsen, Lucas Michels, Luis Felipe Mondini e Thiago Saraiva

# Monitoramento e Previsão de Condições Climáticas

Este projeto tem como objetivo monitorar e prever condições climáticas utilizando um sistema de sensores conectados a um microcontrolador e um modelo de Machine Learning. A seguir, estão descritas as funcionalidades do sistema, os requisitos funcionais (RF), os requisitos não funcionais (RNF), e a arquitetura geral do projeto, incluindo componentes de hardware e software.

## Requisitos Funcionais (RF)

1. **Coleta de Dados Climáticos (RF01):**
   - O sistema deve coletar dados de temperatura, umidade, e pressão atmosférica utilizando os sensores:
     - Sensor DS18B20 para medir a temperatura.
     - Sensor HR202L para medir a umidade relativa do ar.
     - Sensor BMP180 para medir a pressão atmosférica.
   
2. **Transmissão dos Dados (RF02):**
   - O microcontrolador (Arduino) deve transmitir os dados dos sensores para um computador via conexão de rede.

3. **Formatação dos Dados (RF03):**
   - Um script em Python deve formatar os dados recebidos do microcontrolador para um padrão adequado.

4. **Armazenamento dos Dados (RF04):**
   - Os dados formatados devem ser armazenados em uma base de dados MongoDB na nuvem.

5. **Treinamento de Modelo de Machine Learning (RF05):**
   - O sistema deve utilizar a biblioteca Scikit-learn para treinar um modelo de Machine Learning com os dados coletados.
   - O modelo deve ser capaz de prever a probabilidade de chuva com base nas variáveis climáticas.

6. **Apresentação de Resultados (RF06):**
   - O sistema deve apresentar as previsões geradas de maneira clara e acessível para análise e interpretação.

## Requisitos Não Funcionais (RNF)

1. **Desempenho (RNF01):**
   - O sistema deve processar e enviar os dados dos sensores em tempo real, garantindo uma latência mínima entre a coleta e o armazenamento.

2. **Escalabilidade (RNF02):**
   - A solução deve ser capaz de lidar com volumes crescentes de dados e, se necessário, permitir o uso de múltiplos sensores adicionais.

3. **Segurança (RNF03):**
   - A comunicação entre o microcontrolador e o computador, assim como o armazenamento no MongoDB, deve ser segura, com autenticação e criptografia de dados.

4. **Portabilidade (RNF04):**
   - O sistema deve ser modular, permitindo fácil adaptação para novas áreas geográficas ou outras variações de sensores climáticos.

5. **Manutenibilidade (RNF05):**
   - O código deve ser bem documentado e modular, facilitando a manutenção e evolução do sistema ao longo do tempo.

## Arquitetura do Projeto

### Componentes de Hardware

- **Arduino (Microcontrolador):** Responsável por coletar os dados dos sensores e enviá-los ao computador.
- **Sensor DS18B20 (Termômetro):** Mede a temperatura.
- **Sensor HR202L (Higrômetro):** Mede a umidade relativa do ar.
- **Sensor BMP180 (Barômetro):** Mede a pressão atmosférica.
- **Cabo de Rede:** Conecta o microcontrolador ao computador para transmitir os dados.

### Componentes de Software

- **Python:** Utilizado para desenvolver o script que formata os dados e os envia para o MongoDB.
- **MongoDB (Nuvem):** Armazena os dados climáticos coletados.
- **Scikit-learn (Biblioteca de ML):** Utilizada para treinar o modelo de Machine Learning para previsões de chuva.

### Fluxo de Dados

1. **Sensores -> Microcontrolador:** Os dados de temperatura, umidade e pressão são capturados pelos sensores e enviados para o Arduino.
2. **Microcontrolador -> Computador:** O Arduino transmite os dados brutos para um computador via cabo de rede.
3. **Formatação dos Dados:** Um script Python no computador processa os dados, formatando-os em um padrão adequado.
4. **Armazenamento em MongoDB:** Os dados formatados são enviados para o MongoDB, onde são armazenados para uso futuro.
5. **Treinamento de Modelo:** Com os dados armazenados, o modelo de Machine Learning é treinado para prever a possibilidade de chuva.
6. **Previsões:** O modelo realiza previsões baseadas nos dados climáticos, e os resultados são apresentados para o usuário.

### Diagrama de Fluxo

![DiagramaFuncionamento](https://github.com/user-attachments/assets/87dcc0b6-c4e1-4f8a-bf2a-00b4dd171dac)
