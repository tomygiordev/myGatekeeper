#include "Juego.h"
#include <QMessageBox>
#include <QTextStream>
#include <algorithm>
#include <ctime>
#include <regex>

// Helper functions (formerly in main.cpp)
string extraerTextoDeCorchetes(string &linea) {
  regex patron(
      R"(\[(.*?)\])"); // todo lo que hay adentro de los corchetes lo agarra
  smatch coincidencias;
  if (regex_search(linea, coincidencias, patron) && coincidencias.size() > 1) {
    return coincidencias[1].str(); // retorna eso mismo que encontro
  }
  return "";
}

int extraerValorDeCorchetes(string &linea) {
  string texto = extraerTextoDeCorchetes(
      linea); // usa la funcion anterior para ahorrar codigo
  if (!texto.empty()) {
    try {
      return stoi(texto); // como aca necesitamos un valor entero, usamos stoi
                          // para string->int
    } catch (...) {
      return 0;
    }
  }
  return 0;
}

vector<string> extraerVariosTextosCorchetes(const string &linea) {
  regex patron(R"(\[([^\]]+)\])"); //
  sregex_iterator inicio(linea.begin(), linea.end(), patron);
  sregex_iterator fin; // esta funcion es una genialidad, almacena mas de un
                       // valor string por linea
  vector<string> valores;
  for (auto it = inicio; it != fin; ++it) {
    valores.push_back((*it)[1].str()); //
  }
  return valores;
}

// Implementación de la clase Juego
Juego::Juego(QMainWindow *mw)
    : mainWindow(mw), nivelActual(1), errores(0), indicePersonaActual(0),
      personasProcesadasNivel(0), verificacionesRestantes(4),
      rng(std::random_device{}()), logger("logs.txt") {

  // Inicializar puntos por tipo de persona (Roles Skater)
  puntosPorTipo = {
      {"LOCAL LEGEND", 50},  {"PRO SKATER", 30}, {"AMATEUR", 10},
      {"FILMER", 20},        {"GROM", 5},        {"SPONSEE", 15},
      {"POSER", -50},        // Si entra, resta
      {"MALL GRABBER", -30}, // Si entra, resta
      {"SUIT", -100} // GAME OVER logic handled elsewhere or huge penalty
  };
  // Inicializar listas de randomización
  inicializarListas();
  // Cargar nombres y rutas de imágenes
  cargarNombres();
  cargarImagenes();
  // Configurar UI
  configurarUI();
  // Inicializar acumulación de restricciones
  restriccionesAcumuladas.clear();
  // Instalar filtro de eventos para capturar la tecla Escape
  mainWindow->installEventFilter(this);
}

void Juego::inicializarListas() {
  // Crews (Nacionalidades)
  listaCrews = {"Neon Hawks",     "Retro Grinders", "Cyber Rats",
                "Concrete Kings", "Zero Logic",     "Pavement Pirates",
                "Vibe Checkers",  "Glitch Mob",     "Analog Souls",
                "Flipside Locals"};

  // Stances (Estado Civil replacement)
  listaStances = {"Regular", "Goofy", "Mongo"};

  // Skate Styles (Motivo Viaje replacement)
  listaSkateStyles = {"Street", "Vert", "Park", "Cruiser", "Downhill", "Tech"};

  // Session Goals (Tipo Visita replacement)
  listaSessionGoals = {"Filming Part", "Practice",    "Chill Session",
                       "Competition",  "Photo Shoot", "Learning Tricks"};

  // Brands (Pais Residencia replacement)
  listaBrands = {// Core Brands
                 "Deathwish", "Baker", "Zero", "Santa Cruz", "Spitfire",
                 "Independent", "Girl", "Chocolate", "Thrasher", "AntiHero",
                 // Corporate Brands (Acceptable mostly for Pros)
                 "Nike SB", "Adidas", "Red Bull", "Monster",
                 // Mall Brands (Red flag)
                 "Zumiez", "Walmart", "Target", "Decathlon", "Generic"};

  // Deck Conditions
  listaDeckConditions = {"Fresh", "Used", "Thrashed", "Snapped", "Focus"};
}

void Juego::configurarUI() {
  // Crear StackWidget para manejar diferentes pantallas
  stackedWidget = new QStackedWidget(mainWindow);
  mainWindow->setCentralWidget(stackedWidget);

  // Establecer tamaño fijo para la ventana y prevenir redimensionamiento
  mainWindow->setFixedSize(1024, 768);
  // Menú de inicio
  menuInicioWidget = new QWidget();

  // Configura el fondo del menú de inicio
  menuInicioWidget->setStyleSheet(
      "QWidget {"
      "    background-image: url(':/otros/menu.png');"
      "    background-repeat: no-repeat;"
      "    background-position: center;"
      "    background-size: cover;"
      "}");

  QVBoxLayout *menuLayout = new QVBoxLayout(menuInicioWidget);

  jugarButton = new QPushButton("Jugar");
  reglasButton = new QPushButton("Reglas");
  cargarPartidaButton =
      new QPushButton("Cargar Partida"); // **Nuevo botón para cargar partida**
  salirButton = new QPushButton("Salir");

  // Estilizar botones del menú de inicio
  QString botonMenuEstilo =
      "QPushButton {"
      "   background-color: #4CAF50;" // Verde suave
      "   background: #4CAF50;"       // Forzar color de fondo
      "   color: white;"
      "   font-size: 18px;"
      "   padding: 10px;"
      "   border-radius: 8px;"
      "   border: none;" // Remover bordes por defecto
      "}"
      "QPushButton:hover {"
      "   background-color: #45A049;" // Verde un poco más oscuro en hover
      "   background: #45A049;"       // Forzar color de fondo en hover
      "}"
      "QPushButton:pressed {"
      "   background-color: #388E3C;" // Verde oscuro para el estado presionado
      "   background: #388E3C;"
      "}";
  jugarButton->setStyleSheet(botonMenuEstilo);
  reglasButton->setStyleSheet(botonMenuEstilo);
  cargarPartidaButton->setStyleSheet(
      botonMenuEstilo); // **Estilo para cargar partida**
  salirButton->setStyleSheet(botonMenuEstilo);
  // Ajustar tamaños de los botones
  jugarButton->setFixedSize(200, 50);
  reglasButton->setFixedSize(200, 50);
  cargarPartidaButton->setFixedSize(200, 50); // **Tamaño para cargar partida**
  salirButton->setFixedSize(200, 50);
  // Agregar botones al layout con espacio
  menuLayout->addStretch();
  menuLayout->addWidget(jugarButton, 0, Qt::AlignCenter);
  menuLayout->addSpacing(20); // Espacio entre botones
  menuLayout->addWidget(reglasButton, 0, Qt::AlignCenter);
  menuLayout->addSpacing(20); // Espacio entre botones
  menuLayout->addWidget(cargarPartidaButton, 0,
                        Qt::AlignCenter); // **Agregar botón de cargar partida**
  menuLayout->addSpacing(20);             // Espacio entre botones
  menuLayout->addWidget(salirButton, 0, Qt::AlignCenter);
  menuLayout->addStretch();
  // Pantalla de reglas
  reglasWidget = new QWidget();
  QVBoxLayout *reglasLayout = new QVBoxLayout(reglasWidget);

  QTextEdit *reglasTexto = new QTextEdit();
  reglasTexto->setReadOnly(true);
  reglasTexto->setText(obtenerReglasDelJuego());
  reglasTexto->setStyleSheet(
      "font-size: 16px; padding: 10px; background-color: white;");
  volverButton = new QPushButton("Volver al Menú");
  // Estilizar botón de volver
  volverButton->setFixedSize(150, 40);
  volverButton->setStyleSheet(botonMenuEstilo);
  reglasLayout->addWidget(reglasTexto);
  reglasLayout->addSpacing(10);
  reglasLayout->addWidget(volverButton, 0, Qt::AlignCenter);
  // Pantalla de juego
  QWidget *juegoWidget = new QWidget();
  layout = new QVBoxLayout(juegoWidget);
  // Etiquetas de información
  nivelLabel = new QLabel("Nivel: 1");
  puntosLabel = new QLabel("Reputación: 0");
  erroresLabel = new QLabel("Errores: 0");
  resultadoLabel = new QLabel("Resultado:");
  imagenPersonaLabel = new QLabel();
  personaInfo = new QTextEdit();
  personaInfo->setReadOnly(true);
  // Establecer la imagen de fondo
  juegoWidget->setStyleSheet(
      "QWidget {"
      "    background-image: url(':/otros/background.png');"
      "    background-repeat: no-repeat;"
      "    background-position: center;"
      "    background-size: contain;" // Cambia aquí
      "    background-color: white;"  // Fondo sólido detrás de la imagen
      "}");

  // Configurar QLabel para la imagen del personaje
  imagenPersonaLabel->setFixedSize(250, 250); // Tamaño aumentado
  imagenPersonaLabel->setAlignment(Qt::AlignCenter);
  // Opcional: Puedes agregar un marco para mejor visualización
  imagenPersonaLabel->setStyleSheet(
      "QLabel { border: 2px solid black; background-color: white; }");
  // Estilizar etiquetas
  QString etiquetaEstilo = "QLabel { font-size: 16px; background-color: "
                           "transparent; color: black; }";
  nivelLabel->setStyleSheet(etiquetaEstilo);
  puntosLabel->setStyleSheet(etiquetaEstilo);
  erroresLabel->setStyleSheet(etiquetaEstilo);
  resultadoLabel->setStyleSheet(
      "QLabel { font-size: 18px; font-weight: bold; background-color: "
      "transparent; color: black; }");
  personaInfo->setStyleSheet(
      "QTextEdit { font-size: 14px; background-color: white; }");
  // Botones de acción
  aceptarButton = new QPushButton("Aceptar");
  rechazarButton = new QPushButton("Rechazar");
  verRestriccionesButton = new QPushButton("Ver Restricciones");
  verificarPaisRestringidoButton =
      new QPushButton("Verificar País Restringido");
  verificarDocumentacionButton = new QPushButton("Verificar Documentación");
  // Forzar estilo para las etiquetas (niveles, puntos, errores, resultado)
  QString labelEstilo = R"(
    QLabel {
        background-color: white;
        background: white;
        color: black;
        font-size: 16px;
        padding: 5px;
        border-radius: 4px;
        border: 1px solid gray;
    }
)";
  nivelLabel->setStyleSheet(labelEstilo);
  puntosLabel->setStyleSheet(labelEstilo);
  erroresLabel->setStyleSheet(labelEstilo);
  resultadoLabel->setStyleSheet(
      "QLabel { background-color: white; background: white; color: black; "
      "font-size: 18px; padding: 5px; font-weight: bold; border-radius: 4px; "
      "border: 1px solid gray; }");
  QString infoEstilo = R"(
    QTextEdit {
        background-color: white;
        background: white;
        color: black;
        font-size: 14px;
        padding: 5px;
        border-radius: 4px;
        border: 1px solid gray;
    }
)";
  personaInfo->setStyleSheet(infoEstilo);

  // Asegurar que el fondo se rellene completamente en los QLabel
  nivelLabel->setAutoFillBackground(true);
  puntosLabel->setAutoFillBackground(true);
  erroresLabel->setAutoFillBackground(true);
  resultadoLabel->setAutoFillBackground(true);

  // Asegurar que el fondo se rellene completamente en el QTextEdit
  personaInfo->setAutoFillBackground(true);

  // Estilizar botones de acción
  QString botonAccionEstilo = R"(
    QPushButton {
        background-color: #008CBA; /* Azul sólido */
        color: white;
        font-size: 16px;
        padding: 10px;
        border-radius: 8px;
        border: none; /* Remover bordes por defecto */
    }
    QPushButton:hover {
        background-color: #007bb5; /* Azul más oscuro en hover */
    }
    QPushButton:pressed {
        background-color: #005f8a; /* Azul aún más oscuro al presionar */
    }
    QPushButton {
        background: #008CBA; /* Fondo específico para evitar absorción del fondo principal */
  personaInfo->setStyleSheet("font-size: 14px;");

  centerLayout->addWidget(imagenPersonaLabel);
  centerLayout->addWidget(personaInfo);

  // Action Buttons
  QHBoxLayout *actionLayout = new QHBoxLayout();
  aceptarButton = new QPushButton("LET IN (Cool)");
  rechazarButton = new QPushButton("KICK OUT (Poser)");

  aceptarButton->setStyleSheet(
      "background-color: #004400; color: #00ff00; font-size: 20px; "
      "font-weight: bold; border: 2px solid #00ff00;");
  rechazarButton->setStyleSheet(
      "background-color: #440000; color: #ff0000; font-size: 20px; "
      "font-weight: bold; border: 2px solid #ff0000;");

  actionLayout->addWidget(aceptarButton);
  actionLayout->addWidget(rechazarButton);

  // Verification Buttons
  QHBoxLayout *verifyLayout = new QHBoxLayout();
  verificarPaisRestringidoButton = new QPushButton("CHECK CREW REP");
  verificarDocumentacionButton = new QPushButton("CHECK ID / DOCS");
  verRestriccionesButton = new QPushButton("VIEW DAILY ORDERS");

  verifyLayout->addWidget(verificarPaisRestringidoButton);
  verifyLayout->addWidget(verificarDocumentacionButton);
  verifyLayout->addWidget(verRestriccionesButton);

  resultadoLabel = new QLabel("");
  resultadoLabel->setStyleSheet(
      "font-size: 18px; color: #ffff00; font-weight: bold; margin-top: 10px;");
  resultadoLabel->setAlignment(Qt::AlignCenter);

  layout->addLayout(topBar);
  layout->addLayout(centerLayout);
  layout->addWidget(resultadoLabel);
  layout->addLayout(actionLayout);
  layout->addLayout(verifyLayout);

  // Add widgets to Stack
  stackedWidget->addWidget(menuInicioWidget);
  stackedWidget->addWidget(reglasWidget);
  stackedWidget->addWidget(juegoWidget);

  stackedWidget->setCurrentIndex(0); // Start at Menu

  // Connections
  connect(jugarButton, &QPushButton::clicked, this, &Juego::iniciarJuego);
  connect(reglasButton, &QPushButton::clicked, this, &Juego::mostrarReglas);
  connect(salirButton, &QPushButton::clicked, mainWindow, &QMainWindow::close);
  connect(volverButton, &QPushButton::clicked, this, &Juego::volverAlMenu);
  connect(cargarPartidaButton, &QPushButton::clicked, this,
          &Juego::cargarPartida);

  connect(aceptarButton, &QPushButton::clicked, this, &Juego::aceptarPersona);
  connect(rechazarButton, &QPushButton::clicked, this, &Juego::rechazarPersona);
  connect(verificarPaisRestringidoButton, &QPushButton::clicked, this,
          &Juego::verificarPaisRestringido);
  connect(verificarDocumentacionButton, &QPushButton::clicked, this,
          &Juego::verificarDocumentacion);
  connect(verRestriccionesButton, &QPushButton::clicked, this,
          &Juego::mostrarRestricciones);

  // Inicializar sonidos
  sonidoCorrecto.setSource(QUrl("qrc:/sonido/correcto.wav"));
  sonidoCorrecto.setVolume(0.5);

  sonidoError.setSource(QUrl("qrc:/sonido/incorrecto.wav"));
  sonidoError.setVolume(0.5);
  // Configuración del sonido de ambiente
  sonidoAmbiente.setSource(QUrl("qrc:/sonido/ambiente.wav"));
  sonidoAmbiente.setVolume(0.3); // Ajusta el volumen entre 0 y 1
  sonidoAmbiente.setLoopCount(
      QSoundEffect::Infinite); // Loops infinitos para ambiente

  reglasWidget->setStyleSheet(
      "QWidget {"
      "    background-image: url(':/otros/menuReglas.png');"
      "    background-repeat: no-repeat;"
      "    background-position: center;"
      "    background-size: cover;" // Asegura que la imagen cubra todo el
                                    // widget
      "}");
}

QString Juego::obtenerReglasDelJuego() {
  return QString(
      "<h2>Skatetopia Rules</h2>"
      "<p>Bienvenido a <b>Skatetopia</b>, la ciudad secreta. Eres el "
      "<b>Vibe Guard</b>.</p>"
      "<p>Tu misión: Dejar pasar a los que tienen STEEZ y rechazar a "
      "los POSERS.</p>"
      "<h3>Roles:</h3>"
      "<ul>"
      "<li><b>Local Legend (+50):</b> Los reyes del spot. NUNCA "
      "rechazar (salvo regla específica).</li>"
      "<li><b>Pro Skater (+30):</b> Traen prestigio.</li>"
      "<li><b>Amateur (+10):</b> Skaters promedio.</li>"
      "<li><b>Filmer (+20):</b> Esenciales para la cultura.</li>"
      "<li><b>Poser (-50):</b> Falsos. Rechazo INMEDIATO.</li>"
      "<li><b>Mall Grabber (-30):</b> Agarran la tabla por el eje. "
      "Rechazar.</li>"
      "<li><b>Suit (-100):</b> Corporativos o Policias. GAME OVER si "
      "entran 3.</li>"
      "</ul>"
      "<h3>Reglas de Oro (STEEZ):</h3>"
      "<ul>"
      "<li><b>Mongo Push:</b> Si patea con el pie de adelante... "
      "RECHAZAR (salvo Leyendas).</li>"
      "<li><b>Mall Brands:</b> Marcas de centro comercial (Zumiez, "
      "Walmart) -> POSER.</li>"
      "<li><b>Deck Condition:</b> Tabla nueva (Fresh) en un Amateur "
      "es sospechoso.</li>"
      "</ul>"
      "<p>¡Mantén el Vibe real!</p>");
}

void Juego::volverAlMenu() {
  sonidoAmbiente.stop(); // Detener el sonido de ambiente al regresar al menú
  stackedWidget->setCurrentIndex(0); // Volver al menú de inicio
}

void Juego::mostrarReglas() {
  stackedWidget->setCurrentIndex(1); // Mostrar pantalla de reglas
}

void Juego::cargarNombres() {
  // Función lambda para cargar nombres desde un archivo de recursos
  auto cargarArchivoNombres = [&](const QString &ruta,
                                  std::vector<std::string> &nombresDestino,
                                  const std::string &tipo) {
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QMessageBox::critical(
          mainWindow, "Error",
          QString("No se pudo abrir el archivo de nombres de %1.")
              .arg(QString::fromStdString(tipo)));
      return;
    }
    QTextStream in(&archivo);
    while (!in.atEnd()) {
      QString linea = in.readLine().trimmed();
      if (!linea.isEmpty()) {
        nombresDestino.push_back(linea.toStdString());
      }
    }
    archivo.close();
  };

  // Cargar nombres de hombres desde recursos
  cargarArchivoNombres(":/otros/hombreNombre.txt", nombresHombres, "hombres");

  // Cargar nombres de mujeres desde recursos
  cargarArchivoNombres(":/otros/mujerNombre.txt", nombresMujeres, "mujeres");
}

void Juego::cargarImagenes() {
  // Cargar imágenes de hombres
  imagenesHombres = {":/hombre/HOMBRE 2.png",   ":/hombre/HOMBRE A.png",
                     ":/hombre/HOMBRE AA.png",  ":/hombre/HOMBRE B.png",
                     ":/hombre/HOMBRE BB.png",  ":/hombre/HOMBRE C.png",
                     ":/hombre/HOMBRE D.png",   ":/hombre/HOMBRE DD.png",
                     ":/hombre/HOMBRE E.png",   ":/hombre/HOMBRE EE.png",
                     ":/hombre/HOMBRE EEE.png", ":/hombre/HOMBRE F.png",
                     ":/hombre/HOMBRE G.png",   ":/hombre/HOMBRE GH.png",
                     ":/hombre/HOMBRE H.png",   ":/hombre/HOMBRE J.png",
                     ":/hombre/HOMBRE JD.png",  ":/hombre/HOMBRE JJ.png",
                     ":/hombre/HOMBRE K.png",   ":/hombre/HOMBRE L.png",
                     ":/hombre/HOMBRE LL.png",  ":/hombre/HOMBRE M.png",
                     ":/hombre/HOMBRE N.png",   ":/hombre/HOMBRE V.png",
                     ":/hombre/HOMBRE X.png",   ":/hombre/HOMBRE Z.png"};

  // Cargar imágenes de mujeres
  imagenesMujeres = {
      ":/mujer/MUJER 2.png",  ":/mujer/MUJER 3.png",   ":/mujer/MUJER 4.png",
      ":/mujer/MUJER A.png",  ":/mujer/MUJER AAA.png", ":/mujer/MUJER B.png",
      ":/mujer/MUJER E.png",  ":/mujer/MUJER F.png",   ":/mujer/MUJER GG.png",
      ":/mujer/MUJER HH.png", ":/mujer/MUJER J.png",   ":/mujer/MUJER K.png",
      ":/mujer/MUJER R.png",  ":/mujer/MUJER SS.png",  ":/mujer/MUJER.png"};
}

void Juego::cargarConfiguracionNivel(int nivel) {
  nivelConfig = LevelConfig();
  nivelConfig.nivel = nivel;
  acumularReglas.clear();

  switch (nivel) {
  case 1:
    nivelConfig.cantidadPersonas = 5;
    nivelConfig.maxPosersAllowed = 2;
    nivelConfig.duracionMaximaEstancia = 4;
    nivelConfig.bannedGoals.push_back("Filming Part");

    acumularReglas = "Reglas Nivel 1:\n"
                     "- Skaters hoy: 5\n"
                     "- Prohibido: Grabar videos [Filming Part]\n"
                     "- Duración máx: 4 horas\n"
                     "- Máx errores: 2";
    break;

  case 2:
    nivelConfig.cantidadPersonas = 6;
    nivelConfig.maxPosersAllowed = 2;
    nivelConfig.bannedBrands.push_back("Zumiez");
    nivelConfig.bannedStyles.push_back("Downhill");
    nivelConfig.bannedCrews.push_back("Chile");
    nivelConfig.bannedStyles.push_back("Investigacion");

    acumularReglas = "Reglas Nivel 2:\n"
                     "- Skaters hoy: 6\n"
                     "- Prohibido Marca: [Zumiez]\n"
                     "- Prohibido Estilo: [Downhill], [Investigacion]\n"
                     "- Crew Prohibida: [Chile]\n"
                     "- [Competition] solo si < 6 horas\n"
                     "- Máx errores: 2";
    break;

  case 3:
    nivelConfig.cantidadPersonas = 7;
    nivelConfig.maxPosersAllowed = 2;
    nivelConfig.bannedRoles.push_back("GROM");

    acumularReglas = "Reglas Nivel 3:\n"
                     "- Skaters hoy: 7\n"
                     "- Rol Prohibido: [GROM] (No babysitting)\n"
                     "- [Learning Tricks] solo si < 3 horas\n"
                     "- Máx errores: 2";
    break;

  case 4:
    nivelConfig.cantidadPersonas = 7;
    nivelConfig.maxPosersAllowed = 3;
    nivelConfig.bannedRoles.push_back("FILMER");
    nivelConfig.bannedCrews = {"Cyber Rats", "Concrete Kings", "Zero Logic",
                               "Pavement Pirates"};

    acumularReglas = "Reglas Nivel 4:\n"
                     "- Skaters hoy: 7\n"
                     "- Rol Prohibido: [FILMER]\n"
                     "- Prohibido: [SPONSEE] en [Park]\n"
                     "- Prohibido: Crew [Neon Hawks] usando [Nike SB]\n"
                     "- Crews en guerra (BANEADAS): Cyber Rats, Concrete "
                     "Kings, Zero Logic, Pavement Pirates\n"
                     "- Máx errores: 3";
    break;

  case 5:
    nivelConfig.cantidadPersonas = 10;
    nivelConfig.maxPosersAllowed = 1;
    nivelConfig.bannedRoles.push_back("GROM");
    nivelConfig.bannedStances.push_back("Mongo");
    nivelConfig.edad = 16; // Min age

    acumularReglas = "Reglas Nivel 5:\n"
                     "- Skaters hoy: 10\n"
                     "- Prohibido: [GROM]\n"
                     "- Prohibido: [AMATEUR] haciendo [Vert]\n"
                     "- Prohibido: [Vibe Checkers] usando [Walmart]\n"
                     "- Prohibido: [Mongo] Stance (Absoluto)\n"
                     "- Edad Mínima: 16\n"
                     "- [Street] solo si < 40 años\n"
                     "- [Photo Shoot] solo si < 5 horas\n"
                     "- [Downhill] solo si < 5 horas\n"
                     "- Máx errores: 1 (MODO LEYENDA)";
    break;
  default:
    QMessageBox::critical(mainWindow, "Error",
                          QString("Nivel %1 no está definido.").arg(nivel));
    QApplication::quit();
  }
}

void Juego::cargarPersonasNivel(int nivel) {
  personas.clear();
  nombresUsados.clear();
  imagenesUsadas.clear();
  int cantidadPersonas = nivelConfig.cantidadPersonas;

  // Mezclar las listas de nombres y imágenes
  std::shuffle(nombresHombres.begin(), nombresHombres.end(), rng);
  std::shuffle(nombresMujeres.begin(), nombresMujeres.end(), rng);
  std::shuffle(imagenesHombres.begin(), imagenesHombres.end(), rng);
  std::shuffle(imagenesMujeres.begin(), imagenesMujeres.end(), rng);

  size_t nombreIndexH = 0;
  size_t nombreIndexM = 0;
  size_t imagenIndexH = 0;
  size_t imagenIndexM = 0;

  // Distribuciones
  std::uniform_int_distribution<int> distBooleano(0, 1);
  std::uniform_int_distribution<size_t> distCrew(0, listaCrews.size() - 1);
  std::uniform_int_distribution<size_t> distStance(0, listaStances.size() - 1);
  std::uniform_int_distribution<size_t> distBrand(0, listaBrands.size() - 1);
  std::uniform_int_distribution<size_t> distStyle(0,
                                                  listaSkateStyles.size() - 1);
  std::uniform_int_distribution<size_t> distGoal(0,
                                                 listaSessionGoals.size() - 1);
  std::uniform_int_distribution<size_t> distDeck(0, listaDeckConditions.size() -
                                                        1);
  std::uniform_int_distribution<int> distDuration(1, 8); // Horas de sesion

  vector<string> roles = {"LOCAL LEGEND", "PRO SKATER",   "AMATEUR",
                          "FILMER",       "GROM",         "SPONSEE",
                          "POSER",        "MALL GRABBER", "SUIT"};

  // Pesos para los roles (simple logic: random role)
  std::uniform_int_distribution<size_t> distRole(0, roles.size() - 1);

  for (int i = 0; i < cantidadPersonas; ++i) {
    Persona p;
    // Asignar género aleatoriamente
    bool generoAleatorio = (distBooleano(rng) == 0);
    p.setGender(generoAleatorio);

    // Seleccionar nombre
    std::string nombreSeleccionado;
    if (generoAleatorio) { // Hombre
      if (nombreIndexH >= nombresHombres.size())
        nombreIndexH = 0; // Loop if needed
      nombreSeleccionado = nombresHombres[nombreIndexH++];
    } else { // Mujer
      if (nombreIndexM >= nombresMujeres.size())
        nombreIndexM = 0;
      nombreSeleccionado = nombresMujeres[nombreIndexM++];
    }
    p.setName(nombreSeleccionado);

    // Seleccionar imagen
    QString imagenSeleccionada;
    if (generoAleatorio) { // Hombre
      if (imagenIndexH >= imagenesHombres.size())
        imagenIndexH = 0;
      imagenSeleccionada = imagenesHombres[imagenIndexH++];
    } else { // Mujer
      if (imagenIndexM >= imagenesMujeres.size())
        imagenIndexM = 0;
      imagenSeleccionada = imagenesMujeres[imagenIndexM++];
    }
    p.setImagePath(imagenSeleccionada);

    // Asignar atributos base
    string crew = listaCrews[distCrew(rng)];
    string stance = listaStances[distStance(rng)];
    string brand = listaBrands[distBrand(rng)];
    string style = listaSkateStyles[distStyle(rng)];
    string goal = listaSessionGoals[distGoal(rng)];
    string deck = listaDeckConditions[distDeck(rng)];
    string role = roles[distRole(rng)];

    // LOGICA ESPECIFICA DE ROLES (The "STEEZ" Check)

    // 1. LOCAL LEGEND: Nunca Mongo, Marcas Core, Tabla usada
    if (role == "LOCAL LEGEND") {
      if (stance == "Mongo")
        stance = "Regular";
      // Forzar marca core (primeras 10 de la lista)
      std::uniform_int_distribution<size_t> distCore(0, 9);
      brand = listaBrands[distCore(rng)];
      if (deck == "Fresh" || deck == "Snapped")
        deck = "Thrashed";
    }

    // 2. POSER / MALL GRABBER: Alta chance de Mongo, Mall Brands, Texto o Tabla
    // Fresh
    if (role == "POSER" || role == "MALL GRABBER") {
      if (distBooleano(rng))
        stance = "Mongo"; // 50% chance Mongo
      if (distBooleano(rng)) {
        // Forzar marca Mall (ultimas 5)
        std::uniform_int_distribution<size_t> distMall(listaBrands.size() - 5,
                                                       listaBrands.size() - 1);
        brand = listaBrands[distMall(rng)];
      }
      if (distBooleano(rng))
        deck = "Fresh";
    }

    // 3. SUIT: Siempre viene a "Business" (no existe), usamos "Chill Session" o
    // algo sospechoso
    if (role == "SUIT") {
      brand = "Generic";
      style = "Cruiser";
      deck = "Fresh";
      goal = "Observation"; // Custom goal? or stuck to list? Let's stick to
                            // list but look weird.
      // Suits don't skate well using lists, maybe just make them look weird via
      // Deck/Brand
    }

    p.setCrew(crew);
    p.setStance(stance);
    p.setBrand(brand);
    p.setSkateStyle(style);
    p.setSessionGoal(goal);
    p.setDeckCondition(deck);
    p.setSessionDuration(distDuration(rng));
    p.setRole(role);
    p.setHasHelmet(distBooleano(rng) == 0); // 50/50 helmet usage

    // Fecha Nac
    p.setBirthDate(generarFechaAleatoria());
    // p.setDocumentacionValida(distBooleano(rng) == 0); // Removed from logic
    // to rely on rules? Wait, documentation check is usually manual button.
    // Let's set it randomly.
    p.setHasValidID(distBooleano(rng) == 0);

    // Banned Crew logic (reemplaza paisGuerra)
    bool isBanned = false;
    for (const auto &banned : bannedCrews) {
      if (crew == banned) {
        isBanned = true;
        break;
      }
    }
    p.setIsBannedCrew(isBanned);

    personas.push_back(p);
  }
}

void Juego::mostrarPersonaActual() {
  if (indicePersonaActual < personas.size()) {
    const Persona &p = personas[indicePersonaActual];
    verificacionesRestantes = nivelConfig.maxVerificaciones;
    int edad = calcularEdad(p.getBirthDate());
    QString info =
        QString("<b>Name:</b> %1\n").arg(QString::fromStdString(p.getName()));

    info +=
        QString("<b>Rol:</b> %1\n").arg(QString::fromStdString(p.getRole()));
    info +=
        QString("<b>Crew:</b> %1\n").arg(QString::fromStdString(p.getCrew()));

    info += QString("<b>Stance:</b> %1\n")
                .arg(QString::fromStdString(p.getStance()));
    info +=
        QString("<b>Brand:</b> %1\n").arg(QString::fromStdString(p.getBrand()));
    info += QString("<b>Deck:</b> %1\n")
                .arg(QString::fromStdString(p.getDeckCondition()));

    info += QString("<b>Style:</b> %1\n")
                .arg(QString::fromStdString(p.getSkateStyle()));
    info += QString("<b>Goal:</b> %1\n")
                .arg(QString::fromStdString(p.getSessionGoal()));

    info += QString("<b>Edad:</b> %1\n").arg(edad);
    info += QString("<b>Helmet:</b> %1\n").arg(p.getHasHelmet() ? "Yes" : "No");
    info += QString("<b>Duration:</b> %1 hours\n").arg(p.getSessionDuration());

    personaInfo->setHtml(info); // Usar setHtml para formato negrita

    // Cargar imagen
    QPixmap pixmap(p.getImagePath());
    if (!pixmap.isNull()) {
      imagenPersonaLabel->setPixmap(pixmap.scaled(imagenPersonaLabel->size(),
                                                  Qt::KeepAspectRatio,
                                                  Qt::SmoothTransformation));
    } else {
      imagenPersonaLabel->setText("No Image");
    }

    // Actualizar etiquetas
    nivelLabel->setText(QString("Nivel: %1").arg(nivelActual));
    puntosLabel->setText(
        QString("Reputación: %1").arg(vibeCheck.getReputation()));
    erroresLabel->setText(QString("Errores: %1 / %2")
                              .arg(errores)
                              .arg(nivelConfig.maxPosersAllowed));
    resultadoLabel->setText("");

    // Mostrar botones de verificación
    verificarPaisRestringidoButton->setVisible(nivelActual >=
                                               3); // Example condition
    verificarDocumentacionButton->setVisible(nivelActual >= 3);

  } else {
    // Fin del nivel
    mostrarResultadoNivel();
  }
}

void Juego::mostrarRestricciones() {
  QMessageBox::information(mainWindow,
                           "Reglas del Nivel " + QString::number(nivelActual),
                           acumularReglas);
}

string Juego::generarFechaAleatoria() {
  std::uniform_int_distribution<int> distDia(1, 28);
  std::uniform_int_distribution<int> distMes(1, 12);
  std::uniform_int_distribution<int> distAnio(1980, 2009);
  int dia = distDia(rng);
  int mes = distMes(rng);
  int anio = distAnio(rng);
  char buffer[11];
  snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", anio, mes, dia);
  return string(buffer);
}

int Juego::calcularEdad(const string &fechaNac) {
  int anioNacimiento, mesNacimiento, diaNacimiento;
  sscanf(fechaNac.c_str(), "%d-%d-%d", &anioNacimiento, &mesNacimiento,
         &diaNacimiento);

  // Obtener la fecha actual
  time_t t = time(nullptr);
  struct tm *now = localtime(&t);
  int anioActual = now->tm_year + 1900;
  int mesActual = now->tm_mon + 1;
  int diaActual = now->tm_mday;
  int edad = anioActual - anioNacimiento;
  if (mesActual < mesNacimiento ||
      (mesActual == mesNacimiento && diaActual < diaNacimiento)) {
    edad--;
  }
  return edad;
}

void Juego::mostrarResultadoNivel() {
  if (errores <= nivelConfig.maxPosersAllowed) {
    QMessageBox::information(
        mainWindow, "Level Finalized",
        QString("Level %1 Complete!\nReputation: %2\nErrors: %3")
            .arg(nivelActual)
            .arg(vibeCheck.getReputation())
            .arg(errores));
    nivelActual++;
    if (nivelActual > 5) {
      finJuego("You are a SKATE LEGEND! All levels complete.");
    } else {
      cargarConfiguracionNivel(nivelActual);
      cargarPersonasNivel(nivelActual);
      indicePersonaActual = 0;
      personasProcesadasNivel = 0;
      errores = 0;
      mostrarPersonaActual();
    }
  } else {
    finJuego("Level Failed. Too many posers entered.");
  }
}

void Juego::cargarProgreso() {
  nivelActual = 1;
  vibeCheck.setReputation(0);
  errores = 0;
  indicePersonaActual = 0;
  personasProcesadasNivel = 0;
  restriccionesAcumuladas.clear(); // Resetear restricciones acumuladas
}

void Juego::iniciarJuego() {
  // Cambiar a la pantalla de juego
  stackedWidget->setCurrentIndex(2);

  cargarProgreso(); // Reiniciar el progreso
  cargarConfiguracionNivel(nivelActual);
  cargarPersonasNivel(nivelActual);
  mostrarPersonaActual();
  sonidoAmbiente.play();
}

void Juego::finJuego(const std::string &mensaje) {
  sonidoAmbiente.stop();
  QMessageBox::information(mainWindow, "Fin del Juego",
                           QString::fromStdString(mensaje));
  volverAlMenu();
}
bool Juego::evaluarDecision(const Persona &p, bool decision) {
  // 1. Roles Absolutos
  std::string role = p.getRole();

  // SUIT: Must Reject. Game Over logic handled in points (-100), but here
  // we return if decision was correct. If Decision=Allow (True) ->
  // Incorrect. If Decision=Deny (False) -> Correct.
  if (role == "SUIT") {
    return !decision;
  }

  // POSER / MALL GRABBER: Must Reject.
  if (role == "POSER" || role == "MALL GRABBER") {
    return !decision;
  }

  // LOCAL LEGEND: Must Accept (Always).
  if (role == "LOCAL LEGEND") {
    return decision;
  }

  // 2. Reglas del Nivel (Para AMATEUR, PRO, FILMER, GROM, SPONSEE)

  bool shouldReject = false;

  // Check Crew / Banned Crews
  if (p.getCrew() == nivelConfig.bannedCrewStruct)
    shouldReject = true; // mapped from nacionalidadStruct

  // Check against vector of banned crews
  for (const auto &banned : nivelConfig.bannedCrews) {
    if (p.getCrew() == banned)
      shouldReject = true;
  }
  // Also check global banned crews list if still used, but LevelConfig bans
  // are key.
  for (const auto &banned : bannedCrews) {
    if (p.getCrew() == banned)
      shouldReject = true;
  }

  // Check Brand
  for (const auto &banned : nivelConfig.bannedBrands) {
    if (p.getBrand() == banned)
      shouldReject = true;
  }
  if (!nivelConfig.bannedBrandStruct.empty() &&
      p.getBrand() == nivelConfig.bannedBrandStruct)
    shouldReject = true;

  // Check Stance
  for (const auto &banned : nivelConfig.bannedStances) {
    if (p.getStance() == banned)
      shouldReject = true;
  }

  // Check Style
  for (const auto &banned : nivelConfig.bannedStyles) {
    if (p.getSkateStyle() == banned)
      shouldReject = true;
  }

  // Check Deck Condition
  for (const auto &banned : nivelConfig.bannedDeckConditions) {
    if (p.getDeckCondition() == banned)
      shouldReject = true;
  }

  // Check Role
  for (const auto &banned : nivelConfig.bannedRoles) {
    if (p.getRole() == banned)
      shouldReject = true;
  }

  // Check Goal
  for (const auto &banned : nivelConfig.bannedGoals) {
    if (p.getSessionGoal() == banned)
      shouldReject = true;
  }

  // Check Duration
  if (nivelConfig.duracionMaximaEstancia > 0 &&
      p.getSessionDuration() > nivelConfig.duracionMaximaEstancia)
    shouldReject = true;

  // Check Documentation (Siempre requerida valida salvo para leyendas que
  // ya pasaron)
  if (!p.getHasValidID())
    shouldReject = true;

  // Check Age (Min/Max?)
  // Let's assume nivelConfig.edad is a "Min Age for Solo Session"
  if (nivelConfig.edad > 0 &&
      calcularEdad(p.getBirthDate()) < nivelConfig.edad) {
    // Skater kids usually skate together or with parents.
    // Let's say if Age < ConfigAge (e.g. 16) and NO HELMET -> Reject.
    if (!p.getHasHelmet())
      shouldReject = true;
  }

  // 3. Resultado Final
  // If shouldReject is true, Correct Decision is False (Deny).
  // If shouldReject is false, Correct Decision is True (Allow).

  if (shouldReject) {
    return !decision;
  } else {
    return decision;
  }
}

void Juego::aceptarPersona() {
  if (indicePersonaActual < personas.size()) {
    const Persona &p = personas[indicePersonaActual];
    bool decisionCorrecta = evaluarDecision(p, true);

    if (decisionCorrecta) {
      vibeCheck.increaseReputation(puntosPorTipo[p.getRole()]);
      resultadoLabel->setText("¡Aceptado! Keep rollin'.");
      sonidoCorrecto.play();
      logger.log(QString("Aceptar: %1 (%2). Correcto.")
                     .arg(QString::fromStdString(p.getName()))
                     .arg(QString::fromStdString(p.getRole())));
    } else {
      // Pena por error
      int pena = -10; // Default penalty
      if (p.getRole() == "SUIT")
        pena = -100;
      if (p.getRole() == "POSER")
        pena = -50;

      vibeCheck.increaseReputation(pena);
      errores++;
      resultadoLabel->setText("¡Error! Debiste rechazar.");
      sonidoError.play();
      logger.log(QString("Aceptar: %1 (%2). Incorrecto.")
                     .arg(QString::fromStdString(p.getName()))
                     .arg(QString::fromStdString(p.getRole())));
    }

    puntosLabel->setText("Reputación: " +
                         QString::number(vibeCheck.getReputation()));
    erroresLabel->setText("Errores: " + QString::number(errores));

    indicePersonaActual++;
    personasProcesadasNivel++;

    if (errores >= nivelConfig.maxFallosPermitidos) {
      finJuego("Demasiados Posers entraron. El spot fue quemado. Fin del "
               "juego.");
      return;
    }

    if (personasProcesadasNivel >= nivelConfig.cantidadPersonas) {
      nivelActual++;
      if (nivelActual > 5) {
        finJuego("¡Has protegido Skatetopia! Eres una leyenda.");
        return;
      } else {
        cargarConfiguracionNivel(nivelActual);
        cargarPersonasNivel(nivelActual);
        personasProcesadasNivel = 0;
        indicePersonaActual = 0;
        errores = 0;
        QMessageBox::information(mainWindow, "Nivel Completado",
                                 "Nivel Desbloqueado: " +
                                     QString::number(nivelActual));
        mostrarRestricciones();
      }
    }

    mostrarPersonaActual();
  }
}

void Juego::rechazarPersona() {
  if (indicePersonaActual < personas.size()) {
    const Persona &p = personas[indicePersonaActual];
    bool decisionCorrecta = evaluarDecision(p, false);

    if (decisionCorrecta) {
      // Si rechazas correctamente a un Poser/Suit, ganas puntos?
      // O solo no pierdes?
      // Old logic: +puntos if correct.
      // Si era Poser y rechazaste -> Correcto -> +Puntos (e.g. 10 base +
      // bonus?) Use standard points usually positive for correct actions.
      vibeCheck.increaseReputation(20);

      resultadoLabel->setText("¡Rechazado! Bien hecho.");
      sonidoCorrecto.play();
      logger.log(QString("Rechazar: %1 (%2). Correcto.")
                     .arg(QString::fromStdString(p.getName()))
                     .arg(QString::fromStdString(p.getRole())));
    } else {
      // Rechazaste a alguien que debia entrar (ej. Legend)
      vibeCheck.increaseReputation(-20);
      errores++;
      resultadoLabel->setText("¡Error! Ese era real. Debiste aceptar.");
      sonidoError.play();
      logger.log(QString("Rechazar: %1 (%2). Incorrecto.")
                     .arg(QString::fromStdString(p.getName()))
                     .arg(QString::fromStdString(p.getRole())));
    }

    puntosLabel->setText("Reputación: " +
                         QString::number(vibeCheck.getReputation()));
    erroresLabel->setText("Errores: " + QString::number(errores));

    indicePersonaActual++;
    personasProcesadasNivel++;

    if (errores >= nivelConfig.maxFallosPermitidos) {
      finJuego("Demasiados errores. Tu reputación cayó. Fin del juego.");
      return;
    }

    if (personasProcesadasNivel >= nivelConfig.cantidadPersonas) {
      nivelActual++;
      if (nivelActual > 5) {
        finJuego("¡Has protegido Skatetopia! Eres una leyenda.");
        return;
      } else {
        cargarConfiguracionNivel(nivelActual);
        cargarPersonasNivel(nivelActual);
        personasProcesadasNivel = 0;
        indicePersonaActual = 0;
        errores = 0;
        QMessageBox::information(mainWindow, "Nivel Completado",
                                 "Nivel Desbloqueado: " +
                                     QString::number(nivelActual));
        mostrarRestricciones();
      }
    }
    mostrarPersonaActual();
  }
}

void Juego::verificarPaisRestringido() {
  if (nivelActual >= 1 && verificacionesRestantes >
                              0) { // Available from Level 1 now for Crew Check
    const Persona &p = personas[indicePersonaActual];
    verificacionesRestantes--;

    // Check if crew is banned
    bool isBanned = false;
    for (const auto &banned : bannedCrews) {
      if (p.getCrew() == banned)
        isBanned = true;
    }
    // Also check LevelConfig specific bans
    if (p.getCrew() == nivelConfig.bannedCrewStruct)
      isBanned = true;
    for (int i = 0; i < 4; i++) {
      if (!nivelConfig.bannedCrews[i].empty() &&
          p.getCrew() == nivelConfig.bannedCrews[i])
        isBanned = true;
    }

    if (isBanned || p.getIsBannedCrew()) {
      resultadoLabel->setText("ALERTA: Crew en Lista Negra.");
      logger.log(QString("Verificación: Crew %1 Baneada.")
                     .arg(QString::fromStdString(p.getCrew())));
    } else {
      resultadoLabel->setText("Crew limpia.");
      logger.log(QString("Verificación: Crew %1 Limpia.")
                     .arg(QString::fromStdString(p.getCrew())));
    }
  } else {
    resultadoLabel->setText("No tienes más verificaciones.");
  }
}

void Juego::verificarDocumentacion() {
  if (nivelActual >= 1 && verificacionesRestantes > 0) {
    const Persona &p = personas[indicePersonaActual];
    verificacionesRestantes--;

    if (p.getHasValidID()) {
      resultadoLabel->setText("ID Check: VALID (Vibe passed).");
      logger.log("Verificación: Valid ID.");
    } else {
      resultadoLabel->setText("ID Check: FAKE / EXPIRED (Poser).");
      logger.log("Verificación: Invalid ID.");
    }
  } else {
    resultadoLabel->setText("No tienes más verificaciones.");
  }
}
int Juego::obtenerNumeroNiveles() {
  return 5; //
}

// Métodos de serialización
void Juego::saveGame(const QString &filename) {
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly)) {
    QMessageBox::warning(mainWindow, "Guardar Partida",
                         "No se pudo abrir el archivo para guardar.");
    return;
  }

  QDataStream out(&file);
  out.setVersion(QDataStream::Qt_5_15); // Asegurar compatibilidad

  // Guardar VibeCheck (antes ControlFronterizo)
  vibeCheck.serialize(out);

  // Guardar NivelActual, Errores, IndicePersonaActual,
  // PersonasProcesadasNivel, VerificacionesRestantes
  out << nivelActual << errores << indicePersonaActual
      << personasProcesadasNivel << verificacionesRestantes;

  // Guardar restricciones acumuladas
  out << static_cast<quint32>(restriccionesAcumuladas.size());
  for (const auto &restriccion : restriccionesAcumuladas) {
    out << QString::fromStdString(restriccion);
  }

  // Guardar nivelConfig
  nivelConfig.serialize(out);

  // Guardar personas
  out << static_cast<quint32>(personas.size());
  for (const auto &persona : personas) {
    persona.serialize(out);
  }

  file.close();
  QMessageBox::information(mainWindow, "Guardar Partida",
                           "La partida se ha guardado exitosamente.");
  // Registrar el guardado de la partida
  logger.log(QString("Partida guardada en %1.").arg(filename));
}
void Juego::loadGame(const QString &filename) {
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::warning(mainWindow, "Cargar Partida",
                         "No se pudo abrir el archivo para cargar.");
    return;
  }
  QDataStream in(&file);
  in.setVersion(QDataStream::Qt_5_15);
  // Cargar VibeCheck
  vibeCheck.deserialize(in);

  // Cargar NivelActual, Errores, IndicePersonaActual,
  // PersonasProcesadasNivel, VerificacionesRestantes
  in >> nivelActual >> errores >> indicePersonaActual >>
      personasProcesadasNivel >> verificacionesRestantes;

  // Cargar restricciones acumuladas
  quint32 restrSize;
  in >> restrSize;
  restriccionesAcumuladas.clear();
  for (quint32 i = 0; i < restrSize; ++i) {
    QString restriccion;
    in >> restriccion;
    restriccionesAcumuladas.emplace_back(restriccion.toStdString());
  }

  // Cargar nivelConfig
  nivelConfig.deserialize(in);

  // Cargar personas
  quint32 personasSize;
  in >> personasSize;
  personas.clear();
  for (quint32 i = 0; i < personasSize; ++i) {
    Persona p;
    p.deserialize(in);
    personas.push_back(p);
  }

  file.close();

  // Actualizar UI
  nivelLabel->setText("Nivel: " + QString::number(nivelActual));
  puntosLabel->setText("Reputación: " +
                       QString::number(vibeCheck.getReputation()));
  erroresLabel->setText("Errores: " + QString::number(errores));

  mostrarPersonaActual();

  QMessageBox::information(mainWindow, "Cargar Partida",
                           "La partida se ha cargado exitosamente.");
  // Registrar la carga de la partida
  logger.log(QString("Partida cargada desde %1.").arg(filename));
}

bool Juego::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if (keyEvent->key() == Qt::Key_Escape) {
      showInGameMenu();
      return true;
    }
  }
  return QObject::eventFilter(obj, event);
}

void Juego::showInGameMenu() {
  // Crear un diálogo para el menú in-game
  QDialog dialog(mainWindow);
  dialog.setWindowTitle("Menú");
  dialog.setModal(true);
  dialog.setFixedSize(300, 200);

  QVBoxLayout *layoutDialog = new QVBoxLayout(&dialog);

  QPushButton *guardarPartidaBtn = new QPushButton("Guardar Partida");
  QPushButton *volverMenuBtn = new QPushButton("Volver al Menú Principal");
  QPushButton *cerrarJuegoBtn = new QPushButton("Cerrar Juego");

  // Estilizar botones
  QString botonMenuEstilo = "QPushButton {"
                            "   background-color: #4CAF50;"
                            "   color: white;"
                            "   font-size: 16px;"
                            "   padding: 10px;"
                            "   border-radius: 8px;"
                            "   border: none;"
                            "}"
                            "QPushButton:hover {"
                            "   background-color: #45a049;"
                            "}";

  guardarPartidaBtn->setStyleSheet(botonMenuEstilo);
  volverMenuBtn->setStyleSheet(botonMenuEstilo);
  cerrarJuegoBtn->setStyleSheet(botonMenuEstilo);

  // Ajustar tamaños de los botones
  guardarPartidaBtn->setFixedSize(250, 40);
  volverMenuBtn->setFixedSize(250, 40);
  cerrarJuegoBtn->setFixedSize(250, 40);

  // Agregar botones al layout del diálogo con espacio
  layoutDialog->addStretch();
  layoutDialog->addWidget(guardarPartidaBtn, 0, Qt::AlignCenter);
  layoutDialog->addSpacing(20);
  layoutDialog->addWidget(volverMenuBtn, 0, Qt::AlignCenter);
  layoutDialog->addSpacing(20);
  layoutDialog->addWidget(cerrarJuegoBtn, 0, Qt::AlignCenter);
  layoutDialog->addStretch();
  // Conectar señales y slots
  connect(guardarPartidaBtn, &QPushButton::clicked, this,
          &Juego::guardarPartida);
  connect(volverMenuBtn, &QPushButton::clicked, [&]() {
    dialog.accept();
    volverAlMenu();
  });
  connect(cerrarJuegoBtn, &QPushButton::clicked, this, &Juego::cerrarJuego);

  // Mostrar el diálogo
  dialog.exec();
}
void Juego::guardarPartida() {
  QString filename = QFileDialog::getSaveFileName(
      mainWindow, "Guardar Partida", "", "Partida Guardada (*.gam)");
  if (!filename.isEmpty()) {
    if (!filename.endsWith(".gam")) {
      filename += ".gam";
    }
    saveGame(filename);
  }
}

void Juego::cerrarJuego() { QApplication::quit(); }

void Juego::cargarPartida() {
  QString filename = QFileDialog::getOpenFileName(
      mainWindow, "Cargar Partida", "", "Partida Guardada (*.gam)");
  if (!filename.isEmpty()) {
    loadGame(filename);
    stackedWidget->setCurrentIndex(2); // Cambiar a la pantalla de juego
  }
}
