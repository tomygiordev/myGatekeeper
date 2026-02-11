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

// ... (existing includes)

void Juego::configurarUI() {
  // Crear StackWidget para manejar diferentes pantallas
  stackedWidget = new QStackedWidget(mainWindow);
  mainWindow->setCentralWidget(stackedWidget);

  // Establecer tamaño fijo para la ventana y prevenir redimensionamiento
  mainWindow->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint |
                             Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
                             Qt::WindowMinimizeButtonHint |
                             Qt::WindowCloseButtonHint);
  mainWindow->showMaximized();
  // Menú de inicio
  menuInicioWidget = new QWidget();

  // Configura el fondo del menú de inicio
  menuInicioWidget->setStyleSheet(
      "QWidget#menuInicioWidget {"
      "    border-image: url(':/otros/background_mygate.png') 0 0 0 0 stretch "
      "stretch;"
      "}");
  menuInicioWidget->setObjectName("menuInicioWidget");

  QVBoxLayout *menuLayout = new QVBoxLayout(menuInicioWidget);

  jugarButton = new QPushButton("Jugar");
  reglasButton = new QPushButton("Reglas");
  cargarPartidaButton =
      new QPushButton("Cargar Partida"); // **Nuevo botón para cargar partida**
  salirButton = new QPushButton("Salir");

  // --- GLOBAL SKATER STYLE QSS ---
  // Palette:
  // Background: #1a1a1a (Dark Grey/Black)
  // Accent Green: #39FF14 (Neon Green)
  // Accent Pink: #FF007F (Hot Pink)
  // Accent Cyan: #00FFFF (Cyan)
  // Text: White / Black for contrast

  QString globalStyle = R"(
      QMainWindow {
          background-color: #1a1a1a;
      }
      QWidget {
          font-family: 'Segoe UI', Impact, sans-serif;
          font-size: 14px;
          color: #eee;
      }
      /* --- LABELS (Stats & Titles) --- */
      QLabel {
          color: #00FFFF;
          font-weight: bold;
          font-size: 16px;
          background: transparent;
      }
      
      /* --- TEXT INFO PANEL (The Clipboard) --- */
      QTextEdit {
          background-color: #f0f0f0;
          color: #111;
          border: 4px solid #333;
          border-radius: 2px;
          font-family: 'Courier New', monospace;
          font-weight: bold;
          padding: 10px;
          selection-background-color: #FF007F;
          /* Texture effect simulated with color */
      }

      /* --- BUTTONS (Stickers) --- */
      QPushButton {
          background-color: #222;
          color: white;
          border: 3px solid #555;
          border-radius: 5px;
          padding: 8px;
          font-weight: bold;
          text-transform: uppercase;
      }
      QPushButton:hover {
          background-color: #333;
          border-color: #00FFFF; /* Cyan glow */
      }
      QPushButton:pressed {
          background-color: #000;
          border-color: #FF007F;
      }

      /* --- ACTION BUTTONS (Overrides) --- */
      /* These will be set specifically by ID or object name later */
  )";

  // --- RESTORING MISSING MENU LAYOUT LOGIC ---
  // Estilizar botones del menú de inicio (QSS handled globally now, but sizes
  // needed)
  jugarButton->setFixedSize(200, 50);
  reglasButton->setFixedSize(200, 50);
  cargarPartidaButton->setFixedSize(200, 50);
  salirButton->setFixedSize(200, 50);

  // Agregar botones al layout con espacio
  menuLayout->addStretch();
  menuLayout->addWidget(jugarButton, 0, Qt::AlignCenter);
  menuLayout->addSpacing(20);
  menuLayout->addWidget(reglasButton, 0, Qt::AlignCenter);
  menuLayout->addSpacing(20);
  menuLayout->addWidget(cargarPartidaButton, 0, Qt::AlignCenter);
  menuLayout->addSpacing(20);
  menuLayout->addWidget(salirButton, 0, Qt::AlignCenter);
  menuLayout->addStretch();

  // --- RESTORING MISSING REGLAS WIDGET LOGIC ---
  reglasWidget = new QWidget();
  QVBoxLayout *reglasLayout = new QVBoxLayout(reglasWidget);

  QTextEdit *reglasTexto = new QTextEdit();
  reglasTexto->setReadOnly(true);
  reglasTexto->setText(obtenerReglasDelJuego());
  // Override style for rules specifically if needed, or rely on global
  reglasTexto->setStyleSheet(
      "font-size: 16px; padding: 10px; background-color: #eee; color: #111;");

  volverButton = new QPushButton("Volver al Menú");
  volverButton->setFixedSize(200, 50);

  reglasLayout->addWidget(reglasTexto);
  reglasLayout->addSpacing(10);
  reglasLayout->addWidget(volverButton, 0, Qt::AlignCenter);

  // Pantalla de juego
  QWidget *juegoWidget = new QWidget();
  layout = new QVBoxLayout(juegoWidget);

  // --- TOP BAR (Stats) ---
  nivelLabel = new QLabel("Nivel: 1");
  puntosLabel = new QLabel("Reputación: 0");
  erroresLabel = new QLabel("Errores: 0");

  // Custom Style for Top Stats: "Tape" look
  QString tapeStyle = "background-color: #000; color: #39FF14; padding: 5px "
                      "15px; border: 2px dashed #39FF14; border-radius: 0px;";
  nivelLabel->setStyleSheet(tapeStyle);
  puntosLabel->setStyleSheet(tapeStyle);
  erroresLabel->setStyleSheet("background-color: #000; color: #FF007F; "
                              "padding: 5px 15px; border: 2px dashed #FF007F;");

  QHBoxLayout *topBar = new QHBoxLayout();
  topBar->addWidget(nivelLabel);
  topBar->addWidget(puntosLabel);
  topBar->addWidget(erroresLabel);

  // --- MAIN CONTENT (Split View) ---
  QHBoxLayout *centerLayout = new QHBoxLayout();

  // Left Panel: Image
  imagenPersonaLabel = new QLabel();
  imagenPersonaLabel->setFixedSize(300, 300);
  imagenPersonaLabel->setAlignment(Qt::AlignCenter);
  // Polaroids style border
  imagenPersonaLabel->setStyleSheet(
      "border: 10px solid white; border-bottom: 40px solid white; "
      "background-color: #111; color: #555;");
  imagenPersonaLabel->setText("SIN FOTO"); // Default text

  // Right Panel: Info (Clipboard style)
  personaInfo = new QTextEdit();
  personaInfo->setReadOnly(true);
  // Style already set by global QSS, fine-tuning here if needed

  centerLayout->addWidget(imagenPersonaLabel);
  centerLayout->addWidget(personaInfo);

  // --- BOTTOM BAR (Actions) ---

  // Feedback Label
  resultadoLabel = new QLabel("Esperando skater...");
  resultadoLabel->setStyleSheet(
      "font-size: 18px; color: #FF007F; font-weight: 900; background-color: "
      "rgba(0,0,0,0.8); padding: 5px; border: 2px solid white;");
  resultadoLabel->setAlignment(Qt::AlignCenter);

  // Action Buttons
  QHBoxLayout *actionLayout = new QHBoxLayout();
  aceptarButton = new QPushButton("DEJAR PASAR");
  rechazarButton = new QPushButton("EXPULSAR");

  aceptarButton->setMinimumHeight(70);
  rechazarButton->setMinimumHeight(70);

  // Sticker Style - Green
  aceptarButton->setStyleSheet(
      "QPushButton { "
      "   background-color: #39FF14; "
      "   color: black; "
      "   font-size: 24px; "
      "   font-weight: 900; "
      "   border: 3px solid white; "
      "   border-radius: 2px; "
      "   transform: rotate(-2deg);" // Qt QSS doesn't support transform,
                                     // simulating with irregular borders?
                                     // standard simple button for now.
      "}"
      "QPushButton:hover { background-color: #32E012; border: 5px solid white; "
      "}"
      "QPushButton:pressed { background-color: #29B50F; border: 3px solid "
      "black; }");

  // Sticker Style - Red/Pink
  rechazarButton->setStyleSheet("QPushButton { "
                                "   background-color: #FF007F; "
                                "   color: white; "
                                "   font-size: 24px; "
                                "   font-weight: 900; "
                                "   border: 3px solid white; "
                                "   border-radius: 2px; "
                                "}"
                                "QPushButton:hover { background-color: "
                                "#E0006F; border: 5px solid white; }"
                                "QPushButton:pressed { background-color: "
                                "#B5005A; border: 3px solid black; }");

  actionLayout->addWidget(aceptarButton);
  actionLayout->addWidget(rechazarButton);

  // Verification Buttons (Smaller)
  QHBoxLayout *verifyLayout = new QHBoxLayout();
  verificarPaisRestringidoButton = new QPushButton("Verificar Crew");
  verificarDocumentacionButton = new QPushButton("Verificar DNI");
  verRestriccionesButton = new QPushButton("Órdenes Diarias");

  QString verifyStyle =
      "QPushButton { "
      "   background-color: #00FFFF; "
      "   color: black; "
      "   font-weight: bold; "
      "   border: 2px solid black; "
      "   padding: 10px; "
      "}"
      "QPushButton:hover { background-color: #E0FFFF; }"
      "QPushButton:pressed { background-color: #008B8B; color: white; }";

  verificarPaisRestringidoButton->setStyleSheet(verifyStyle);
  verificarDocumentacionButton->setStyleSheet(verifyStyle);
  verRestriccionesButton->setStyleSheet(verifyStyle);

  verifyLayout->addWidget(verificarPaisRestringidoButton);
  verifyLayout->addWidget(verificarDocumentacionButton);
  verifyLayout->addWidget(verRestriccionesButton);

  // Add all to Main Layout
  layout->addLayout(topBar);
  layout->addLayout(centerLayout); // Image + Info
  layout->addWidget(resultadoLabel);
  layout->addLayout(actionLayout); // Big Buttons
  layout->addLayout(verifyLayout); // Small Buttons

  // Styles are applied directly to widgets, removing old "starStyle"

  // Background - Keep Image or Solid Color?
  // Let's rely on the MainWindow background, but maybe add transparency to the
  // widget
  juegoWidget->setStyleSheet(
      "QWidget#juegoWidget { background: transparent; }");
  juegoWidget->setObjectName("juegoWidget");

  // Add widgets to Stack
  stackedWidget->addWidget(menuInicioWidget);
  stackedWidget->addWidget(reglasWidget);
  stackedWidget->addWidget(juegoWidget);

  stackedWidget->setCurrentIndex(0); // Start at Menu

  // --- MENU STYLING ---
  // Apply sticker style to menu buttons too
  QString menuBtnStyle = "QPushButton { "
                         "   background-color: #111; "
                         "   color: #39FF14; "
                         "   font-size: 22px; " // Larger
                         "   font-weight: bold; "
                         "   border: 2px solid #39FF14; "
                         "   padding: 15px; "
                         "   border-radius: 0px; "
                         "}"
                         "QPushButton:hover { "
                         "   background-color: #39FF14; "
                         "   color: black; "
                         "}"
                         "QPushButton:pressed { "
                         "   background-color: #29B50F; "
                         "   color: black; "
                         "}";

  jugarButton->setStyleSheet(menuBtnStyle);
  reglasButton->setStyleSheet(menuBtnStyle);
  cargarPartidaButton->setStyleSheet(menuBtnStyle);
  salirButton->setStyleSheet(
      "QPushButton { background-color: #111; color: #FF007F; font-size: 22px; "
      "font-weight: bold; border: 2px solid #FF007F; padding: 15px; } "
      "QPushButton:hover { background-color: #FF007F; color: white; }");
  volverButton->setStyleSheet(menuBtnStyle);

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
      "QWidget#reglasWidget {"
      "    border-image: url(':/otros/menuReglas.png') 0 0 0 0 stretch stretch;"
      "}");
  reglasWidget->setObjectName("reglasWidget");
}

QString Juego::obtenerReglasDelJuego() {
  return QString(
      "<h2>Reglas de Skatetopia</h2>"
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
      "<li><b>Suit (-100):</b> Corporativos o Policías. GAME OVER si "
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

  // Cargar apellidos
  cargarArchivoNombres(":/otros/apellidos.txt", apellidos, "apellidos");
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
  // Mezclar apellidos también
  std::shuffle(apellidos.begin(), apellidos.end(), rng);

  size_t nombreIndexH = 0;
  size_t nombreIndexM = 0;
  size_t apellidoIndex = 0; // Índice para apellidos
  size_t imagenIndexH = 0;
  size_t imagenIndexM = 0;

  auto getRandomSurname = [&]() -> std::string {
    if (apellidos.empty())
      return "";
    std::string apellido = apellidos[apellidoIndex];
    apellidoIndex = (apellidoIndex + 1) % apellidos.size();
    if (apellidoIndex == 0)
      std::shuffle(apellidos.begin(), apellidos.end(),
                   rng); // Re-shuffle if exhausted
    return apellido;
  };

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

  // Pesos para los roles: Favor Valid Roles
  // Valid: LOCAL LEGEND, PRO SKATER, AMATEUR, FILMER, GROM, SPONSEE (Indices
  // 0-5) Invalid: POSER, MALL GRABBER, SUIT (Indices 6-8)

  // Chance of Valid Role: 70% (Level 1) -> Decreases slightly by level?
  // Let's keep it simple: 70% Valid / 30% Invalid

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

    // --- WEIGHTED ROLE GENERATION (Dynamic Difficulty) ---
    std::string role;
    bool isValidRole = false;

    // Calculate a random threshold between 50 and 70 for this specific skater
    // This makes the difficulty feel more "organic" and less predictable than a
    // fixed 70%
    int validityThreshold = std::uniform_int_distribution<int>(50, 70)(rng);
    int roll = std::uniform_int_distribution<int>(0, 100)(rng);

    if (roll < validityThreshold) { // Dynamic Valid %
      // Pick from first 6 roles (Valid)
      std::uniform_int_distribution<size_t> distValid(0, 5);
      role = roles[distValid(rng)];
      isValidRole = true;
    } else { // Remainder Invalid
      // Pick from last 3 roles (Invalid)
      std::uniform_int_distribution<size_t> distInvalid(6, 8);
      role = roles[distInvalid(rng)];
      isValidRole = false;
    }
    p.setRole(role);

    // Asignar atributos base (SMART GENERATION)
    string crew = listaCrews[distCrew(rng)]; // Random Crew mostly fine

    string stance, brand, style, goal, deck;

    if (isValidRole) {
      // Generate mostly Valid attributes to avoid accidental bans
      // Avoid "Mongo" (Index for standard Stance list needs checking, assuming
      // Mongo is distinct) Hardcode safe values or retry if banned

      stance = (distBooleano(rng)) ? "Regular" : "Goofy"; // Safe

      // Brand: Pick from first 10 (Core brands)
      std::uniform_int_distribution<size_t> distCore(0, 9);
      brand = listaBrands[distCore(rng)];

      // Style: avoid "Mall Grab" if that's a style
      style = "Street";

      // Goal: "Filming Part" or "Session"
      goal = "Session";

      deck = "Thrashed"; // Skaters have used decks
    } else {
      // Invalid Role -> Make it obvious (Mall Grab, Mongo, Poser brands)
      stance = "Mongo";
      brand = "Zumiez"; // Example 'bad' brand
      style = "Mall Grab";
      goal = "Loitering";
      deck = "Fresh"; // Posers have new decks

      // Randomize slightly so not all are identical
      if (distBooleano(rng))
        deck = "Walmart Board";
    }

    // Override for specific Logic if needed (like Local Legend always perfect)
    if (role == "LOCAL LEGEND") {
      brand = "Zero"; // Example core
      deck = "Thrashed";
      stance = "Goofy";
    }

    // Logic for specific "Hard" levels can be added here later.

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
    // Actualizar UI
    // Keep technical terms in English (Stance, Crew, Brand)
    // Translate labels: Name -> Nombre, Age -> Edad, Goal -> Objetivo
    QString info = QString("<b>Nombre:</b> %1 "
                           "<b>Role:</b> %2<br>"
                           "<b>Crew:</b> %3 "
                           "<b>Stance:</b> %4<br>"
                           "<b>Brand:</b> %5 "
                           "<b>Deck:</b> %6<br>"
                           "<b>Style:</b> %7 "
                           "<b>Objetivo:</b> %8<br>"
                           "<b>Edad:</b> %9 "
                           "<b>Casco:</b> %10<br>"
                           "<b>Duración:</b> %11 horas")
                       .arg(QString::fromStdString(p.getName()))
                       .arg(QString::fromStdString(p.getRole()))
                       .arg(QString::fromStdString(p.getCrew()))
                       .arg(QString::fromStdString(p.getStance()))
                       .arg(QString::fromStdString(p.getBrand()))
                       .arg(QString::fromStdString(p.getDeckCondition()))
                       .arg(QString::fromStdString(p.getSkateStyle()))
                       .arg(QString::fromStdString(p.getSessionGoal()))
                       .arg(calcularEdad(p.getBirthDate()))
                       .arg(p.getHasHelmet() ? "Sí" : "No")
                       .arg(p.getSessionDuration());

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
        mainWindow, "Nivel Finalizado",
        QString("¡Nivel %1 Completado!\nReputación: %2\nErrores: %3")
            .arg(nivelActual)
            .arg(vibeCheck.getReputation())
            .arg(errores));
    nivelActual++;
    if (nivelActual > 5) {
      finJuego("¡Eres una LEYENDA! Todos los niveles completados.");
    } else {
      cargarConfiguracionNivel(nivelActual);
      cargarPersonasNivel(nivelActual);
      indicePersonaActual = 0;
      personasProcesadasNivel = 0;
      errores = 0;
      mostrarPersonaActual();
    }
  } else {
    finJuego("Fallaste el nivel. Demasiados posers.");
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
  lastRejectionReason.clear();
  std::vector<std::string> reasons;

  // 1. Roles Absolutos
  std::string role = p.getRole();

  if (role == "SUIT") {
    reasons.push_back("Es SUIT (Policía/Corp) - Rechazo Mandatorio.");
  }

  if (role == "POSER" || role == "MALL GRABBER") {
    reasons.push_back("Es POSER/MALL GRABBER - Se nota falso.");
  }

  // 2. Reglas del Nivel

  // Check Crew / Banned Crews
  if (p.getCrew() == nivelConfig.bannedCrewStruct)
    reasons.push_back("Crew Prohibida (Config Struct): " + p.getCrew());

  for (const auto &banned : nivelConfig.bannedCrews) {
    if (p.getCrew() == banned)
      reasons.push_back("Crew Prohibida (Lista): " + p.getCrew());
  }
  for (const auto &banned : bannedCrews) {
    if (p.getCrew() == banned)
      reasons.push_back("Crew en Guerra (Global): " + p.getCrew());
  }

  // Check Brand
  for (const auto &banned : nivelConfig.bannedBrands) {
    if (p.getBrand() == banned)
      reasons.push_back("Marca Prohibida: " + p.getBrand());
  }
  if (!nivelConfig.bannedBrandStruct.empty() &&
      p.getBrand() == nivelConfig.bannedBrandStruct)
    reasons.push_back("Marca Prohibida (Config): " + p.getBrand());

  // Check Stance
  for (const auto &banned : nivelConfig.bannedStances) {
    if (p.getStance() == banned)
      reasons.push_back("Stance Prohibido: " + p.getStance());
  }

  // Check Style
  for (const auto &banned : nivelConfig.bannedStyles) {
    if (p.getSkateStyle() == banned)
      reasons.push_back("Estilo Prohibido: " + p.getSkateStyle());
  }

  // Check Deck Condition
  for (const auto &banned : nivelConfig.bannedDeckConditions) {
    if (p.getDeckCondition() == banned)
      reasons.push_back("Tabla Prohibida: " + p.getDeckCondition());
  }

  // Check Role
  for (const auto &banned : nivelConfig.bannedRoles) {
    if (p.getRole() == banned)
      reasons.push_back("Rol Prohibido en este nivel: " + p.getRole());
  }

  // Check Goal
  for (const auto &banned : nivelConfig.bannedGoals) {
    if (p.getSessionGoal() == banned)
      reasons.push_back("Objetivo Prohibido: " + p.getSessionGoal());
  }

  // Check Duration
  if (nivelConfig.duracionMaximaEstancia > 0 &&
      p.getSessionDuration() > nivelConfig.duracionMaximaEstancia)
    reasons.push_back("Excede duración máxima: " +
                      std::to_string(p.getSessionDuration()) + "h");

  // Check Documentation
  if (!p.getHasValidID())
    reasons.push_back("Documentación Inválida/Falsa.");

  // Check Age
  if (nivelConfig.edad > 0 &&
      calcularEdad(p.getBirthDate()) < nivelConfig.edad) {
    if (!p.getHasHelmet())
      reasons.push_back("Menor de edad sin casco.");
  }

  // LOGIC DECISION
  // shouldReject is true if ANY reason exists (except for Local Legend override
  // below)
  bool shouldReject = !reasons.empty();

  // LOCAL LEGEND Override: Always Allowed unless game over condition?
  // Specs say "Local Legend: Kings... NEVER reject".
  if (role == "LOCAL LEGEND") {
    shouldReject = false;
    reasons.clear(); // Clear reasons because Legend passes anyway.
  }

  if (shouldReject) {
    // Build debug string
    for (const auto &r : reasons) {
      lastRejectionReason += QString::fromStdString(r) + "\n";
    }
    // Correct action is to Deny (False).
    // If decision is Allow (True) -> Returns False (Incorrect)
    return !decision;
  } else {
    // Correct action is to Allow (True).
    // If decision is Allow (True) -> Returns True (Correct)
    return decision;
  }
}

void Juego::aceptarPersona() {
  if (indicePersonaActual < personas.size()) {
    const Persona &p = personas[indicePersonaActual];
    bool decisionCorrecta = evaluarDecision(p, true);

    if (decisionCorrecta) {
      vibeCheck.increaseReputation(10);
      resultadoLabel->setText("¡Bienvenido! +Reputación");
      sonidoCorrecto.play();
      logger.log(QString("Aceptar: %1 (%2). Correcto.")
                     .arg(QString::fromStdString(p.getName()))
                     .arg(QString::fromStdString(p.getRole())));
    } else {
      vibeCheck.increaseReputation(-20);
      errores++;

      // Mostrar por que NO debiste aceptarlo
      if (!lastRejectionReason.isEmpty()) {
        resultadoLabel->setText("¡ERROR! Debiste rechazarlo por:\n" +
                                lastRejectionReason);
      } else {
        resultadoLabel->setText("¡Error! Debiste rechazarlo.");
      }

      sonidoError.play();
      logger.log(
          QString("Aceptar: %1 (%2). Incorrecto. Debio ser rechazado por: %3")
              .arg(QString::fromStdString(p.getName()))
              .arg(QString::fromStdString(p.getRole()))
              .arg(lastRejectionReason));
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
      if (!lastRejectionReason.isEmpty()) {
        resultadoLabel->setText("¡Incorrecto! Razones ocultas:\n" +
                                lastRejectionReason);
      } else {
        resultadoLabel->setText("¡Error! Ese era real. Debiste aceptar.");
      }
      sonidoError.play();
      logger.log(QString("Rechazar: %1 (%2). Incorrecto. Razon: %3")
                     .arg(QString::fromStdString(p.getName()))
                     .arg(QString::fromStdString(p.getRole()))
                     .arg(lastRejectionReason));
    }

    puntosLabel->setText("Reputación: " +
                         QString::number(vibeCheck.getReputation()));
    erroresLabel->setText("Errores: " + QString::number(errores));

    indicePersonaActual++;
    personasProcesadasNivel++;

    if (errores >= nivelConfig.maxPosersAllowed) {
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
      resultadoLabel->setText("DNI: VÁLIDO (Todo en orden).");
      logger.log("Verificación: Valid ID.");
    } else {
      resultadoLabel->setText("DNI: FALSO / INVALIDO.");
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
