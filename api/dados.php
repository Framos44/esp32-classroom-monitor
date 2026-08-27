<?php
	$dadosRecebidos = file_get_contents('php://input');
	$dados = json_decode($dadosRecebidos, true);

	$temperatura = $dados['temperatura'];
	$umidade = $dados['umidade'];
	$luminosidade = $dados['luminosidade'];
	$qualidade_ar = $dados['qualidade_ar'];
	$nivel_som = $dados['nivel_som'];

	$host = "localhost";
	$dbname = "xxxx";
	$usuario = "xxxx";
	$senha = "xxxx";

	try {
		$pdo = new PDO("mysql:host=$host;dbname=$dbname;charset=utf8", $usuario, $senha);
		$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	} catch (PDOException $e) {
    	die("Erro de conexao: " . $e->getMessage());
	}

	$sql = "INSERT INTO leituras (temperatura, umidade, luminosidade, qualidade_ar, nivel_som)
        VALUES (:temperatura, :umidade, :luminosidade, :qualidade_ar, :nivel_som)";

	$stmt = $pdo->prepare($sql);
	$stmt->bindParam(':temperatura', $temperatura);
	$stmt->bindParam(':umidade', $umidade);
	$stmt->bindParam(':luminosidade', $luminosidade);
	$stmt->bindParam(':qualidade_ar', $qualidade_ar);
	$stmt->bindParam(':nivel_som', $nivel_som);

	$stmt->execute();

	echo json_encode(["status" => "sucesso", "mensagem" => "Dados salvos com sucesso"]);
