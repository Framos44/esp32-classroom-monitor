CREATE TABLE leituras (
  id INT AUTO_INCREMENT PRIMARY KEY,
  temperatura FLOAT NOT NULL,
  umidade FLOAT NOT NULL,
  luminosidade INT NOT NULL,
  qualidade_ar INT NOT NULL,
  nivel_som INT NOT NULL,
  criado_em DATETIME DEFAULT CURRENT_TIMESTAMP
);
