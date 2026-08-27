<?php
	$host = "localhost";
	$dbname = "xxxxx";
	$usuario = "xxxx";
	$senha = "xxxxxx";
	try {
		$pdo = new PDO("mysql:host=$host;dbname=$dbname;charset=utf8", $usuario, $senha);
		$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	} catch (PDOException $e) {
		die("Erro de conexao: " . $e->getMessage());
	}
	$sql = "SELECT * FROM leituras ORDER BY criado_em DESC LIMIT 1";
	$stmt = $pdo->prepare($sql);
	$stmt->execute();
	$ultimaLeitura = $stmt->fetch(PDO::FETCH_ASSOC);

	// Classificação do nível de claridade
	if ($ultimaLeitura['luminosidade'] <= 1365) {
		$claridadeTexto = "Muito claro";
	} else if ($ultimaLeitura['luminosidade'] <= 2730) {
		$claridadeTexto = "Adequado";
	} else {
		$claridadeTexto = "Escuro";
	}

	// Classificação da qualidade do ar
	if ($ultimaLeitura['qualidade_ar'] <= 3132) {
		$qualidadeArTexto = "Bom";
	} else if ($ultimaLeitura['qualidade_ar'] <= 3665) {
		$qualidadeArTexto = "Ruim";
	} else {
		$qualidadeArTexto = "Extremo";
	}

	// Classificação do nível de som
	if ($ultimaLeitura['nivel_som'] <= 1500) {
		$nivelSomTexto = "Baixo";
	} else if ($ultimaLeitura['nivel_som'] <= 3000) {
		$nivelSomTexto = "Moderado";
	} else {
		$nivelSomTexto = "Alto";
	}
?>

<!DOCTYPE html>
<html lang="pt-br">
<head>
	<link rel="stylesheet" href="estilo.css">
    <meta charset="UTF-8">
    <title>Monitoramento Ambiental</title>
</head>
	<body>
		<h1>Monitoramento Ambiental</h1>
		<p class="subtitulo">Dados em tempo real da sala de aula</p>

		<div class="container">
			<div class="card">
				<div class="icone">🌡️</div>
				<h2>Temperatura</h2>
				<p class="valor"><?php echo $ultimaLeitura['temperatura']; ?>°C</p>
			</div>
			<div class="card">
				<div class="icone">💧</div>
				<h2>Umidade</h2>
				<p class="valor"><?php echo $ultimaLeitura['umidade']; ?>%</p>
			</div>
			<div class="card">
				<div class="icone">💡</div>
				<h2>Luminosidade</h2>
				<p class="valor"><?php echo $claridadeTexto ?></p>
			</div>
			<div class="card">
				<div class="icone">🌬️</div>
				<h2>Qualidade do ar</h2>
				<p class="valor"><?php echo $qualidadeArTexto; ?></p>
			</div>
			<div class="card">
				<div class="icone">🔊</div>
				<h2>Nível de som</h2>
				<p class="valor"><?php echo $nivelSomTexto; ?></p>
			</div>
			<div class="card atualizacao">
				<div class="icone">🕒</div>
				<h2>Última atualização</h2>
				<p class="valor"><?php echo $ultimaLeitura['criado_em']; ?></p>
			</div>
		</div>
	</body>
</html>
