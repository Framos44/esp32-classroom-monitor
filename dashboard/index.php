<?php
	$host = "localhost";
	$dbname = "Micro";
	$usuario = "xxxx";
	$senha = "xxxx";
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
		$claridadeClasse = "atencao";
	} else if ($ultimaLeitura['luminosidade'] <= 2730) {
		$claridadeTexto = "Adequado";
		$claridadeClasse = "bom";
	} else {
		$claridadeTexto = "Escuro";
		$claridadeClasse = "atencao";
	}

	// Classificação da qualidade do ar
	if ($ultimaLeitura['qualidade_ar'] <= 3132) {
		$qualidadeArTexto = "Bom";
		$qualidadeArClasse = "bom";
	} else if ($ultimaLeitura['qualidade_ar'] <= 3665) {
		$qualidadeArTexto = "Ruim";
		$qualidadeArClasse = "atencao";
	} else {
		$qualidadeArTexto = "Extremo";
		$qualidadeArClasse = "critico";
	}

	// Classificação do nível de som
	if ($ultimaLeitura['nivel_som'] <= 1500) {
		$nivelSomTexto = "Baixo";
		$nivelSomClasse = "bom";
	} else if ($ultimaLeitura['nivel_som'] <= 3000) {
		$nivelSomTexto = "Moderado";
		$nivelSomClasse = "atencao";
	} else {
		$nivelSomTexto = "Alto";
		$nivelSomClasse = "critico";
	}
?>

<!DOCTYPE html>
<html lang="pt-br">
<head>
	<meta http-equiv="refresh" content="30">
	<link rel="stylesheet" href="estilo.css">
    <meta charset="UTF-8">
    <title>Monitoramento Ambiental</title>
	<link rel="icon" type="image/svg+xml" href="imagens/logo2.svg">
</head>
	<body>
		<div class="cabecalho">
			<img src="imagens/logo3.svg" alt="Logo SIMAEP" class="logo">
			<div>
				<h1>Monitoramento Ambiental</h1>
				<p class="subtitulo">Dados em tempo real da sala de aula</p>
			</div>
		</div>

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
				<p class="valor <?php echo $claridadeClasse; ?>"><?php echo $claridadeTexto; ?></p>
			</div>
			<div class="card">
				<div class="icone">🌬️</div>
				<h2>Qualidade do ar</h2>
				<p class="valor <?php echo $qualidadeArClasse; ?>"><?php echo $qualidadeArTexto; ?></p>
			</div>
			<div class="card">
				<div class="icone">🔊</div>
				<h2>Nível de som</h2>
				<p class="valor <?php echo $nivelSomClasse; ?>"><?php echo $nivelSomTexto; ?></p>
			</div>
			<div class="card atualizacao">
				<div class="icone">🕒</div>
				<h2><span class="pulso"></span> Última atualização</h2>
				<p class="valor"><?php echo $ultimaLeitura['criado_em']; ?></p>
			</div>
		</div>
	</body>
</html>
