# Padrão de Driver para os Sensores I2C da Estação Meteorológica

O pafrão estabelece que:  
* drivers I2C sempre retornam um ErrorType  
    -- O driver não pode assumir que a comunicação sempre deu certo. Toda função que conversa com hardware deve indicar se houve sucesso ou falha, para que o software acima possa reagir (repetir, logar, ignorar, entrar em modo degradado, etc).  
* dados vão por referência:  
    -Em vez de retornar valores diretamente, a função recebe ponteiros (ex: Aht10Data *out). Assim, ela pode preencher vários campos ao mesmo tempo (umidade, temperatura etc), e retornar o status via ErrorType.  
* sem floats no nível driver  
    - Dados float são causam lentidão, ocupam mais MEMÓEIA e embaralham o driver com dependência de math.h. O driver deve apresentar dados crus convertidos para inteiros escalonados. Quem quiser float, converte fora.  
* unidades fixas (escalas)  
    - Se cada driver inventa uma escala, o software fica inconsistente. Se existe uma escala padrão (ex: décimos de % e centésimos de °C), todos os drivers entregam exatamente nesse formato e o restante do sistema pode confiar.  
    - por exemplo:  

    | Grandeza | Tipo | Escala |
    | -------- | ----- | ------- |
    | Umidade | int16 | décimos % (456 → 45.6%) |
    | Temp | int16 | centésimos °C (2534 → 25.34°C) |

Além disso, o ErrorType precisa ser global na estação. Isso quer dizer: um error.h comum, sem dependência circular.  
Os drivers drivers precisam ter, sempre que possível, uma função init e uma função read, no máximo.
Se isso for implantado, o padrão exige que:
```c
ErrorType Foo_init(void);
ErrorType Foo_read(FooData *data);
```
Em que FooData é uma struct de tipos simples e escalas fixas.  
E Foo_init é nomeDoSensor_init() ou nomeDoSensor_read(), por exemplo.  
A ideia é que o driver entregue os dados já convertidos na escala definida para o projeto.  
Os daos “raw” só ficam internos ao driver.  
Erros tratados:  
| Enum	| Quando usar |
| -------- | ----------------------------------- |
| NO_ERROR	| leitura ok |
| NO_ACK	| sensor não respondeu na I2C |
| TIMEOUT_ERROR	| sensor não respondeu no tempo |
| INVALID_VALUE	| pacote/protocolo inválido |
| CRC_ERROR	CRC | não bateu (sensores que usam) |
| OUT_OF_RANGE	| valor plausível de protocolo mas impossível fisicamente |










