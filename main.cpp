#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <sstream>
#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QMessageBox>
#include <QString>
#include <QPixmap>
#include <QFile>
#include <QSoundEffect>
#include <QDir>
#include <QTextStream>
#include <QStackedWidget>
#include <QObject>
#include <QSizePolicy>
#include <QDialog>
#include <QDataStream>
#include <QFileDialog>
#include <QKeyEvent>
#include <QDateTime>
#include <regex> //funciones de corchetes, STL

using namespace std;

// Clase Logger para manejar el registro de eventos
class Logger {
private:
    QFile logFile;
    QTextStream logStream;

public:
    Logger(const QString& filePath) {
        logFile.setFileName(filePath);
        if (logFile.open(QIODevice::Append | QIODevice::Text)) {
            logStream.setDevice(&logFile);
        } else {
            QMessageBox::warning(nullptr, "Error de Log", "No se pudo abrir el archivo de log.");
        }
    }
    ~Logger() {
        if (logFile.isOpen()) {
            logFile.close();
        }
    }
    void log(const QString& message) {
        if (logFile.isOpen()) {
            QString timeStamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
            logStream << "[" << timeStamp << "] " << message << "\n";
            logStream.flush();
        }
    }
};
// Clase Persona
class Persona {
private:
    string nombreApellido;
    string nacionalidad;
    string fechaNac;
    string TipoV;
    string ECivil;
    string paisResi;
    string motivo;
    int duracion;
    bool viajaSolo;
    string tipoPersona;
    bool documentacionValida;
    bool paisRestringido;
    bool genero; // true: hombre, false: mujer
    QString rutaImagen; // Ruta de la imagen asociada

public:
    Persona();
    // Constructor con parámetros
    Persona(const string& nomApe, const string& nac, const string& fecha, const string& tipoVis, const string& estCivil,
            const string& paisResidencia, const string& proposito, int dur, bool solo, const string& tipoPers,
            bool docValida, bool paisRestring, bool gen, const QString& imgRuta);

    // Métodos de acceso
    string obtenerNombreApellido() const;
    string obtenerNacionalidad() const;
    string obtenerFechaNac() const;
    string obtenerTipoV() const;
    int obtenerDuracion() const;
    string obtenerECivil() const;
    string obtenerPaisResi() const;
    string obtenerMotivo() const;
    bool obtenerViajaSolo() const;
    string getTipoPersona() const;
    bool obtenerPaisRestringido() const;
    bool obtenerDocumentacionValida() const;
    void setGenero(bool gen);
    bool getGenero() const;
    void setRutaImagen(const QString& ruta);
    QString getRutaImagen() const;

    // Métodos modificadores
    void setNombreApellido(const std::string& nombreApellido_);
    void setNacionalidad(const std::string& nacionalidad_);
    void setFechaNac(const std::string& fechaNac_);
    void setTipoV(const std::string& tipoV_);
    void setECivil(const std::string& eCivil_);
    void setPaisResi(const std::string& paisResi_);
    void setMotivo(const std::string& motivo_);
    void setDuracion(int duracion_);
    void setViajaSolo(bool viajaSolo_);
    void setTipoPersona(const std::string& tipoPersona_);
    void setDocumentacionValida(bool docValida_);
    void setPaisRestringido(bool paisRestringido_);

    // Métodos de serialización
    void serialize(QDataStream& out) const;
    void deserialize(QDataStream& in);
};

// Implementación de la clase Persona
Persona::Persona() {}

Persona::Persona(const string& nomApe, const string& nac, const string& fecha, const string& tipoVis, const string& estCivil,
                 const string& paisResidencia, const string& proposito, int dur, bool solo, const string& tipoPers,
                 bool docValida, bool paisRestring, bool gen, const QString& imgRuta)
    : nombreApellido(nomApe), nacionalidad(nac), fechaNac(fecha), TipoV(tipoVis), ECivil(estCivil), paisResi(paisResidencia),
    motivo(proposito), duracion(dur), viajaSolo(solo), tipoPersona(tipoPers), documentacionValida(docValida),
    paisRestringido(paisRestring), genero(gen), rutaImagen(imgRuta) {}

// Métodos de acceso
string Persona::obtenerNombreApellido() const { return nombreApellido; }
string Persona::obtenerNacionalidad() const { return nacionalidad; }
string Persona::obtenerFechaNac() const { return fechaNac; }
string Persona::obtenerTipoV() const { return TipoV; }
int Persona::obtenerDuracion() const { return duracion; }
string Persona::obtenerECivil() const { return ECivil; }
string Persona::obtenerPaisResi() const { return paisResi; }
string Persona::obtenerMotivo() const { return motivo; }
bool Persona::obtenerViajaSolo() const { return viajaSolo; }
string Persona::getTipoPersona() const { return tipoPersona; }
bool Persona::obtenerPaisRestringido() const { return paisRestringido; }
bool Persona::obtenerDocumentacionValida() const { return documentacionValida; }
void Persona::setGenero(bool gen) { genero = gen; }
bool Persona::getGenero() const { return genero; }
void Persona::setRutaImagen(const QString& ruta) { rutaImagen = ruta; }
QString Persona::getRutaImagen() const { return rutaImagen; }

// Métodos modificadores
void Persona::setNombreApellido(const std::string& nombreApellido_) { nombreApellido = nombreApellido_; }
void Persona::setNacionalidad(const std::string& nacionalidad_) { nacionalidad = nacionalidad_; }
void Persona::setFechaNac(const std::string& fechaNac_) { fechaNac = fechaNac_; }
void Persona::setTipoV(const std::string& tipoV_) { TipoV = tipoV_; }
void Persona::setECivil(const std::string& eCivil_) { ECivil = eCivil_; }
void Persona::setPaisResi(const std::string& paisResi_) { paisResi = paisResi_; }
void Persona::setMotivo(const std::string& motivo_) { motivo = motivo_; }
void Persona::setDuracion(int duracion_) { duracion = duracion_; }
void Persona::setViajaSolo(bool viajaSolo_) { viajaSolo = viajaSolo_; }
void Persona::setTipoPersona(const std::string& tipoPersona_) { tipoPersona = tipoPersona_; }
void Persona::setDocumentacionValida(bool docValida_) { documentacionValida = docValida_; }
void Persona::setPaisRestringido(bool paisRestringido_) { paisRestringido = paisRestringido_; }
// Métodos de serialización
void Persona::serialize(QDataStream& out) const {
    out << QString::fromStdString(nombreApellido)
        << QString::fromStdString(nacionalidad)
        << QString::fromStdString(fechaNac)
        << QString::fromStdString(TipoV)
        << QString::fromStdString(ECivil)
        << QString::fromStdString(paisResi)
        << QString::fromStdString(motivo)
        << duracion
        << viajaSolo
        << QString::fromStdString(tipoPersona)
        << documentacionValida
        << paisRestringido
        << genero
        << rutaImagen;
}
void Persona::deserialize(QDataStream& in) {
    QString nomApe, nac, fecha, tipoVis, estCivil, paisResidencia, proposito, tipoPers, imgRuta;
    in >> nomApe
        >> nac
        >> fecha
        >> tipoVis
        >> estCivil
        >> paisResidencia
        >> proposito
        >> duracion
        >> viajaSolo
        >> tipoPers
        >> documentacionValida
        >> paisRestringido
        >> genero
        >> imgRuta;

    nombreApellido = nomApe.toStdString();
    nacionalidad = nac.toStdString();
    fechaNac = fecha.toStdString();
    TipoV = tipoVis.toStdString();
    ECivil = estCivil.toStdString();
    paisResi = paisResidencia.toStdString();
    motivo = proposito.toStdString();
    tipoPersona = tipoPers.toStdString();
    rutaImagen = imgRuta;
}

// Clase ControlFronterizo
class ControlFronterizo {
private:
    int puntos;

public:
    ControlFronterizo();

    int getPuntos() const;
    void setPuntos(int pts);
    void incrementarPuntos(int pts);
    void decrementarPuntos(int pts);

    // Métodos de serialización
    void serialize(QDataStream& out) const;
    void deserialize(QDataStream& in);
};
// Implementación de ControlFronterizo
ControlFronterizo::ControlFronterizo() : puntos(0) {}
int ControlFronterizo::getPuntos() const { return puntos; }
void ControlFronterizo::setPuntos(int pts) { puntos = pts; }
void ControlFronterizo::incrementarPuntos(int pts) { puntos += pts; }
void ControlFronterizo::decrementarPuntos(int pts) { puntos -= pts; }
// Métodos de serialización
void ControlFronterizo::serialize(QDataStream& out) const {
    out << puntos;
}
void ControlFronterizo::deserialize(QDataStream& in) {
    in >> puntos;
}
// Clase LevelConfig para leer la configuración de cada nivel
class LevelConfig {
public:
    int nivel;
    int maxErrores;
    int maxVerificaciones;
    int personasPorNivel;
    int cantidadPersonas;
    string tipoVisitaRestringido;
    int duracionMaximaEstancia;
    int maxFallosPermitidos;
    string reglasTexto;
    string motivoViajeRestringido;
    string paisRechazar;
    string tipoPersonaRechazar;
    string nacionalidadStruct;
    string residenciaStruct;
    string paisesEnGuerra[4];
    string estadoAmor;
    int edad;
    vector<string> restricciones;
    LevelConfig();
    // Métodos de serialización
    void serialize(QDataStream& out) const;
    void deserialize(QDataStream& in);
};
// Implementación de LevelConfig
LevelConfig::LevelConfig() : nivel(0), maxErrores(0), maxVerificaciones(3), personasPorNivel(0) {}

// Métodos de serialización
void LevelConfig::serialize(QDataStream& out) const {
    out << nivel << maxErrores << maxVerificaciones << personasPorNivel;
    out << static_cast<quint32>(restricciones.size());
    for (const auto& restriccion : restricciones) {
        out << QString::fromStdString(restriccion);
    }
}
void LevelConfig::deserialize(QDataStream& in) {
    in >> nivel >> maxErrores >> maxVerificaciones >> personasPorNivel;
    quint32 size;
    in >> size;
    restricciones.clear();
    for (quint32 i = 0; i < size; ++i) {
        QString restriccion;
        in >> restriccion;
        restricciones.emplace_back(restriccion.toStdString());
    }
}
// Clase Juego
class Juego : public QObject {
    Q_OBJECT
private:
    ControlFronterizo control;
    vector<Persona> personas;
    vector<string> paisesGuerra;
    int nivelActual=1;
    int errores;
    int indicePersonaActual;
    int personasProcesadasNivel;
    int verificacionesRestantes;
    unordered_map<string, int> puntosPorTipo;
    QString acumularReglas;
    // Configuración del nivel
    LevelConfig nivelConfig;
    // Registro de eventos
    vector<string> registroEventos;
    // UI
    QMainWindow* mainWindow;
    QLabel* resultadoLabel;
    QLabel* nivelLabel;
    QLabel* puntosLabel;
    QLabel* erroresLabel;
    QLabel* imagenPersonaLabel;
    QTextEdit* personaInfo;
    QPushButton* aceptarButton;
    QPushButton* rechazarButton;
    QPushButton* verificarPaisRestringidoButton;
    QPushButton* verificarDocumentacionButton;
    QPushButton* verRestriccionesButton; // **Nuevo botón para restricciones**
    QPushButton* guardarButton; // Botón de guardar en el menú in-game
    QPushButton* volverMenuButton; // Botón para volver al menú principal
    QPushButton* cerrarJuegoButton; // Botón para cerrar el juego
    QVBoxLayout* layout;
    // Menú de inicio y reglas
    QWidget* menuInicioWidget;
    QWidget* reglasWidget;
    QPushButton* jugarButton;
    QPushButton* reglasButton;
    QPushButton* salirButton;
    QPushButton* cargarPartidaButton; // **Nuevo botón para cargar partida**
    QPushButton* volverButton;
    QStackedWidget* stackedWidget;
    // Sonidos
    QSoundEffect sonidoCorrecto;
    QSoundEffect sonidoError;
    QSoundEffect sonidoAmbiente;
    // Listas de nombres y rutas de imágenes
    vector<string> nombresHombres;
    vector<string> nombresMujeres;
    vector<QString> imagenesHombres;
    vector<QString> imagenesMujeres;
    // Conjuntos para evitar repeticiones
    unordered_set<string> nombresUsados;
    unordered_set<QString> imagenesUsadas;
    // Listas para randomización
    vector<string> listaPaisesAmerica;
    vector<string> listaEstadosCiviles;
    vector<string> listaMotivosViaje;
    vector<string> listaTiposVisita;
    // Acumulación de restricciones
    vector<string> restriccionesAcumuladas;
    // Generador de números aleatorios
    std::mt19937 rng;
    // Logger
    Logger logger; // **Nuevo miembro para el logger**

public:
    Juego(QMainWindow* mw);

    // Métodos
    void inicializarListas();
    void configurarUI();
    QString obtenerReglasDelJuego();
    void volverAlMenu();
    void mostrarReglas();
    void cargarNombres();
    void cargarImagenes();
    void cargarConfiguracionNivel(int nivel);
    void cargarPersonasNivel(int nivel);
    void mostrarPersonaActual();
    string generarFechaAleatoria();
    int calcularEdad(const string& fechaNac);
    void cargarProgreso();
    void finJuego(const std::string& mensaje);
    bool evaluarDecision(const Persona& p, bool decision);
    int obtenerNumeroNiveles();
    // Métodos de serialización
    void saveGame(const QString& filename);
    void loadGame(const QString& filename);
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

public slots:
    void aceptarPersona();
    void rechazarPersona();
    void verificarPaisRestringido();
    void verificarDocumentacion();
    void mostrarRestricciones(); // **Nuevo slot para mostrar restricciones**
    void iniciarJuego(); // **Declaración agregada**
    void showInGameMenu(); // **Nuevo slot para mostrar menú in-game**
    void guardarPartida(); // **Nuevo slot para guardar partida**
    void cerrarJuego(); // **Nuevo slot para cerrar juego**
    void cargarPartida(); // **Nuevo slot para cargar partida**
};

// Implementación de la clase Juego
Juego::Juego(QMainWindow* mw)
    : mainWindow(mw), nivelActual(1), errores(0), indicePersonaActual(0),
    personasProcesadasNivel(0), verificacionesRestantes(4), rng(std::random_device{}()),
    logger("logs.txt") { // **Inicialización del logger con el archivo de log**

    // Inicializar puntos por tipo de persona
    puntosPorTipo = {
        {"ALDEANO", 10},
        {"REFUGIADO", 15},
        {"DIPLOMATICO", 25},
        {"REVOLUCIONARIO", 25}
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
    listaPaisesAmerica = {
        "Argentina", "Bolivia", "Brasil", "Chile", "Colombia",
        "Costa Rica", "Cuba", "Ecuador", "El Salvador", "Guatemala",
        "Honduras", "Mexico", "Nicaragua", "Panamá", "Paraguay",
        "Perú", "República Dominicana", "Uruguay", "Venezuela",
        "Canadá", "Estados Unidos"
    };
    // Lista de estados civiles
    listaEstadosCiviles = {"Soltero", "Casado", "Divorciado", "Viudo"};
    // Lista de motivos de viaje
    listaMotivosViaje = {"Vacaciones", "Conferencia", "Atencion medica", "Investigacion", "Evento Deportivo"};
    // Lista de tipos de visita
    listaTiposVisita = {"Turismo", "Negocios", "Visita Familiar", "Estudios", "Trabajo"};
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
    menuInicioWidget->setStyleSheet("QWidget {"
                                    "    background-image: url(':/otros/menu.png');"
                                    "    background-repeat: no-repeat;"
                                    "    background-position: center;"
                                    "    background-size: cover;"
                                    "}");

    QVBoxLayout* menuLayout = new QVBoxLayout(menuInicioWidget);

    jugarButton = new QPushButton("Jugar");
    reglasButton = new QPushButton("Reglas");
    cargarPartidaButton = new QPushButton("Cargar Partida"); // **Nuevo botón para cargar partida**
    salirButton = new QPushButton("Salir");

    // Estilizar botones del menú de inicio
    QString botonMenuEstilo =
        "QPushButton {"
        "   background-color: #4CAF50;"  // Verde suave
        "   background: #4CAF50;"        // Forzar color de fondo
        "   color: white;"
        "   font-size: 18px;"
        "   padding: 10px;"
        "   border-radius: 8px;"
        "   border: none;"               // Remover bordes por defecto
        "}"
        "QPushButton:hover {"
        "   background-color: #45A049;"  // Verde un poco más oscuro en hover
        "   background: #45A049;"        // Forzar color de fondo en hover
        "}"
        "QPushButton:pressed {"
        "   background-color: #388E3C;"  // Verde oscuro para el estado presionado
        "   background: #388E3C;"
        "}";
    jugarButton->setStyleSheet(botonMenuEstilo);
    reglasButton->setStyleSheet(botonMenuEstilo);
    cargarPartidaButton->setStyleSheet(botonMenuEstilo); // **Estilo para cargar partida**
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
    menuLayout->addWidget(cargarPartidaButton, 0, Qt::AlignCenter); // **Agregar botón de cargar partida**
    menuLayout->addSpacing(20); // Espacio entre botones
    menuLayout->addWidget(salirButton, 0, Qt::AlignCenter);
    menuLayout->addStretch();
    // Pantalla de reglas
    reglasWidget = new QWidget();
    QVBoxLayout* reglasLayout = new QVBoxLayout(reglasWidget);

    QTextEdit* reglasTexto = new QTextEdit();
    reglasTexto->setReadOnly(true);
    reglasTexto->setText(obtenerReglasDelJuego());
    reglasTexto->setStyleSheet("font-size: 16px; padding: 10px; background-color: white;");
    volverButton = new QPushButton("Volver al Menú");
    // Estilizar botón de volver
    volverButton->setFixedSize(150, 40);
    volverButton->setStyleSheet(botonMenuEstilo);
    reglasLayout->addWidget(reglasTexto);
    reglasLayout->addSpacing(10);
    reglasLayout->addWidget(volverButton, 0, Qt::AlignCenter);
    // Pantalla de juego
    QWidget* juegoWidget = new QWidget();
    layout = new QVBoxLayout(juegoWidget);
    // Etiquetas de información
    nivelLabel = new QLabel("Nivel: 1");
    puntosLabel = new QLabel("Puntos: 0");
    erroresLabel = new QLabel("Errores: 0");
    resultadoLabel = new QLabel("Resultado:");
    imagenPersonaLabel = new QLabel();
    personaInfo = new QTextEdit();
    personaInfo->setReadOnly(true);
    // Establecer la imagen de fondo
    juegoWidget->setStyleSheet("QWidget {"
                               "    background-image: url(':/otros/background.png');"
                               "    background-repeat: no-repeat;"
                               "    background-position: center;"
                               "    background-size: contain;" // Cambia aquí
                               "    background-color: white;" // Fondo sólido detrás de la imagen
                               "}");

    // Configurar QLabel para la imagen del personaje
    imagenPersonaLabel->setFixedSize(250, 250); // Tamaño aumentado
    imagenPersonaLabel->setAlignment(Qt::AlignCenter);
    // Opcional: Puedes agregar un marco para mejor visualización
    imagenPersonaLabel->setStyleSheet("QLabel { border: 2px solid black; background-color: white; }");
    // Estilizar etiquetas
    QString etiquetaEstilo = "QLabel { font-size: 16px; background-color: transparent; color: black; }";
    nivelLabel->setStyleSheet(etiquetaEstilo);
    puntosLabel->setStyleSheet(etiquetaEstilo);
    erroresLabel->setStyleSheet(etiquetaEstilo);
    resultadoLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; background-color: transparent; color: black; }");
    personaInfo->setStyleSheet("QTextEdit { font-size: 14px; background-color: white; }");
    // Botones de acción
    aceptarButton = new QPushButton("Aceptar");
    rechazarButton = new QPushButton("Rechazar");
    verRestriccionesButton = new QPushButton("Ver Restricciones");
    verificarPaisRestringidoButton = new QPushButton("Verificar País Restringido");
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
    resultadoLabel->setStyleSheet("QLabel { background-color: white; background: white; color: black; font-size: 18px; padding: 5px; font-weight: bold; border-radius: 4px; border: 1px solid gray; }");
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
    }
)";
    // Aplica el estilo a cada botón de acción
    aceptarButton->setStyleSheet(botonAccionEstilo);
    rechazarButton->setStyleSheet(botonAccionEstilo);
    verRestriccionesButton->setStyleSheet(botonAccionEstilo);
    verificarPaisRestringidoButton->setStyleSheet(botonAccionEstilo);
    verificarDocumentacionButton->setStyleSheet(botonAccionEstilo);

    // Ajustar tamaños de los botones
    aceptarButton->setFixedSize(120, 40);
    rechazarButton->setFixedSize(120, 40);
    verRestriccionesButton->setFixedSize(150, 40);
    verificarPaisRestringidoButton->setFixedSize(200, 40);
    verificarDocumentacionButton->setFixedSize(200, 40);

    // Añadir etiquetas al layout con espacio
    layout->addWidget(nivelLabel, 0, Qt::AlignCenter);
    layout->addWidget(puntosLabel, 0, Qt::AlignCenter);
    layout->addWidget(erroresLabel, 0, Qt::AlignCenter);
    layout->addSpacing(10);
    layout->addWidget(imagenPersonaLabel, 0, Qt::AlignCenter);
    layout->addSpacing(10);
    layout->addWidget(personaInfo, 0, Qt::AlignCenter);
    layout->addSpacing(10);
    layout->addWidget(resultadoLabel, 0, Qt::AlignCenter);
    layout->addSpacing(20);

    // Añadir botones de decisión y el nuevo botón en el orden correcto
    QHBoxLayout* botonesLayout = new QHBoxLayout;
    botonesLayout->addStretch();
    botonesLayout->addWidget(aceptarButton);
    botonesLayout->addSpacing(20); // Espacio entre botones
    botonesLayout->addWidget(verRestriccionesButton);
    botonesLayout->addSpacing(20); // Espacio entre botones
    botonesLayout->addWidget(rechazarButton);
    botonesLayout->addStretch();
    layout->addLayout(botonesLayout);
    layout->addSpacing(20);

    // Añadir botones de verificación
    QHBoxLayout* verificacionesLayout = new QHBoxLayout;
    verificacionesLayout->addStretch();
    verificacionesLayout->addWidget(verificarPaisRestringidoButton);
    verificacionesLayout->addSpacing(20); // Espacio entre botones
    verificacionesLayout->addWidget(verificarDocumentacionButton);
    verificacionesLayout->addStretch();
    layout->addLayout(verificacionesLayout);

    // Aplicar márgenes y espaciado al layout principal
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    // Agregar widgets al stackedWidget
    stackedWidget->addWidget(menuInicioWidget);     // Índice 0
    stackedWidget->addWidget(reglasWidget);         // Índice 1
    stackedWidget->addWidget(juegoWidget);          // Índice 2

    // Conectar señales y slots
    connect(jugarButton, &QPushButton::clicked, this, &Juego::iniciarJuego);
    connect(reglasButton, &QPushButton::clicked, this, &Juego::mostrarReglas);
    connect(salirButton, &QPushButton::clicked, mainWindow, &QMainWindow::close);
    connect(volverButton, &QPushButton::clicked, this, &Juego::volverAlMenu);
    connect(cargarPartidaButton, &QPushButton::clicked, this, &Juego::cargarPartida); // **Conexión para cargar partida**

    connect(aceptarButton, &QPushButton::clicked, this, &Juego::aceptarPersona);
    connect(rechazarButton, &QPushButton::clicked, this, &Juego::rechazarPersona);
    connect(verRestriccionesButton, &QPushButton::clicked, this, &Juego::mostrarRestricciones);
    connect(verificarPaisRestringidoButton, &QPushButton::clicked, this, &Juego::verificarPaisRestringido);
    connect(verificarDocumentacionButton, &QPushButton::clicked, this, &Juego::verificarDocumentacion);

    // Inicializar sonidos
    sonidoCorrecto.setSource(QUrl("qrc:/sonido/correcto.wav"));
    sonidoCorrecto.setVolume(0.5);

    sonidoError.setSource(QUrl("qrc:/sonido/incorrecto.wav"));
    sonidoError.setVolume(0.5);
    // Configuración del sonido de ambiente
    sonidoAmbiente.setSource(QUrl("qrc:/sonido/ambiente.wav"));
    sonidoAmbiente.setVolume(0.3); // Ajusta el volumen entre 0 y 1
    sonidoAmbiente.setLoopCount(QSoundEffect::Infinite); // Loops infinitos para ambiente

    reglasWidget->setStyleSheet(
        "QWidget {"
        "    background-image: url(':/otros/menuReglas.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    background-size: cover;"  // Asegura que la imagen cubra todo el widget
        "}"
        );

}

QString Juego::obtenerReglasDelJuego() {
    return QString(
        "<h2>Reglas del Juego</h2>"
        "<p>El objetivo es revisar las solicitudes de entrada y tomar decisiones precisas para ganar puntos y evitar multas.</p>"
        "<h3>Tipos de Personas:</h3>"
        "<ul>"
        "<li><b>Aldeano:</b> +10 puntos si autorizas correctamente, -15 puntos si te equivocas.</li>"
        "<li><b>Refugiado político:</b> +15 puntos si autorizas correctamente, -25 puntos si te equivocas.</li>"
        "<li><b>Diplomático:</b> +25 puntos si autorizas correctamente, -25 puntos y multa si te equivocas.</li>"
        "<li><b>Revolucionario:</b> Nunca debe entrar. -25 puntos y multa si te equivocas.</li>"
        "</ul>"
        "<h3>Restricciones Adicionales:</h3>"
        "<p>Se aplicarán restricciones adicionales en cada nivel.</p>"
        "<p><b>¡Buena suerte!</b></p>"
        );
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
    auto cargarArchivoNombres = [&](const QString& ruta, std::vector<std::string>& nombresDestino, const std::string& tipo) {
        QFile archivo(ruta);
        if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(mainWindow, "Error", QString("No se pudo abrir el archivo de nombres de %1.").arg(QString::fromStdString(tipo)));
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
    imagenesHombres = {
        ":/hombre/HOMBRE 2.png",
        ":/hombre/HOMBRE A.png",
        ":/hombre/HOMBRE AA.png",
        ":/hombre/HOMBRE B.png",
        ":/hombre/HOMBRE BB.png",
        ":/hombre/HOMBRE C.png",
        ":/hombre/HOMBRE D.png",
        ":/hombre/HOMBRE DD.png",
        ":/hombre/HOMBRE E.png",
        ":/hombre/HOMBRE EE.png",
        ":/hombre/HOMBRE EEE.png",
        ":/hombre/HOMBRE F.png",
        ":/hombre/HOMBRE G.png",
        ":/hombre/HOMBRE GH.png",
        ":/hombre/HOMBRE H.png",
        ":/hombre/HOMBRE J.png",
        ":/hombre/HOMBRE JD.png",
        ":/hombre/HOMBRE JJ.png",
        ":/hombre/HOMBRE K.png",
        ":/hombre/HOMBRE L.png",
        ":/hombre/HOMBRE LL.png",
        ":/hombre/HOMBRE M.png",
        ":/hombre/HOMBRE N.png",
        ":/hombre/HOMBRE V.png",
        ":/hombre/HOMBRE X.png",
        ":/hombre/HOMBRE Z.png"
    };

    // Cargar imágenes de mujeres
    imagenesMujeres = {
        ":/mujer/MUJER 2.png",
        ":/mujer/MUJER 3.png",
        ":/mujer/MUJER 4.png",
        ":/mujer/MUJER A.png",
        ":/mujer/MUJER AAA.png",
        ":/mujer/MUJER B.png",
        ":/mujer/MUJER E.png",
        ":/mujer/MUJER F.png",
        ":/mujer/MUJER GG.png",
        ":/mujer/MUJER HH.png",
        ":/mujer/MUJER J.png",
        ":/mujer/MUJER K.png",
        ":/mujer/MUJER R.png",
        ":/mujer/MUJER SS.png",
        ":/mujer/MUJER.png"
    };
}
//regex y smatch son funciones STL
string extraerTextoDeCorchetes(string& linea) {
    regex patron(R"(\[(.*?)\])"); // todo lo que hay adentro de los corchetes lo agarra
    smatch coincidencias;
    if (regex_search(linea, coincidencias, patron) && coincidencias.size() > 1) {
        return coincidencias[1].str(); // retorna eso mismo que encontro
    }
}
int extraerValorDeCorchetes(string& linea) {
    string texto = extraerTextoDeCorchetes(linea); // usa la funcion anterior para ahorrar codigo
    if (!texto.empty()) {
        return stoi(texto); // como aca necesitamos un valor entero, usamos stoi para string->int
    }
}
vector<string> extraerVariosTextosCorchetes(const string& linea) {
    regex patron(R"(\[([^\]]+)\])"); //
    sregex_iterator inicio(linea.begin(), linea.end(), patron);
    sregex_iterator fin; //esta funcion es una genialidad, almacena mas de un valor string por linea
    vector<string> valores;
    for (auto it = inicio; it != fin; ++it) {
        valores.push_back((*it)[1].str()); //
    }
    return valores;
}

void Juego::cargarConfiguracionNivel(int nivel) {
    nivelConfig = LevelConfig();
    QString rutaReglas = QString(":/nivel%1/reglasNivel%1.txt").arg(nivel);
    QFile archivoReglas(rutaReglas);
    acumularReglas.clear();

    if (!archivoReglas.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(mainWindow, "Error", QString("No se pudo cargar el archivo de reglas del nivel %1.").arg(nivel));
        QApplication::quit();
    }

    QTextStream in(&archivoReglas);
    int linea = 0;
    while (!in.atEnd()) {
        QString reglaQt = in.readLine().trimmed();
        std::string regla = reglaQt.toStdString();
        if (regla.empty()) continue;

        switch (nivel) {
        case 1: { //NIVEL 1
            switch (linea) {
            case 0:
                nivelConfig.cantidadPersonas = extraerValorDeCorchetes(regla);
                break;
            case 1:
                nivelConfig.tipoVisitaRestringido = extraerTextoDeCorchetes(regla);
                break;
            case 2:
                nivelConfig.duracionMaximaEstancia = extraerValorDeCorchetes(regla);
                break;
            case 3:
                nivelConfig.maxFallosPermitidos = extraerValorDeCorchetes(regla);
                break;
            }
            break;
        }
        case 2: { //NIVEL 2
            switch (linea) {
            case 0:
                nivelConfig.cantidadPersonas = extraerValorDeCorchetes(regla);
                break;
            case 1:
            {
                std::vector<std::string> valores = extraerVariosTextosCorchetes(regla);
                if (valores.size() >= 2) {
                    nivelConfig.tipoVisitaRestringido = valores[0];
                    nivelConfig.duracionMaximaEstancia = stoi(valores[1]);
                }
            }
            break;
            case 2:
                nivelConfig.paisRechazar = extraerTextoDeCorchetes(regla);
                break;
            case 3:
                nivelConfig.motivoViajeRestringido = extraerTextoDeCorchetes(regla);
                break;
            case 4:
                nivelConfig.maxFallosPermitidos = extraerValorDeCorchetes(regla);
                break;
            }
            break;
        }
        case 3: { //NIVEL 3
            switch (linea) {
            case 0:
                nivelConfig.cantidadPersonas = extraerValorDeCorchetes(regla);
                break;
            case 1:
            // Supongamos que esta línea es una regla adicional sin extraer valores específicos
            {

            }
            break;
            case 2:
                nivelConfig.tipoPersonaRechazar = extraerTextoDeCorchetes(regla);
                break;
            case 3:
            {
                std::vector<std::string> valores = extraerVariosTextosCorchetes(regla);
                if (valores.size() >= 2) {
                    nivelConfig.tipoVisitaRestringido = valores[0];
                    nivelConfig.duracionMaximaEstancia = std::stoi(valores[1]);
                }
            }
            break;
            case 4:
                nivelConfig.maxFallosPermitidos = extraerValorDeCorchetes(regla);
                break;
            }
            break;
        }
        case 4: { //NIVEL 4
            switch (linea) {
            case 0:
                nivelConfig.cantidadPersonas =extraerValorDeCorchetes(regla);
                break;
            case 1:
                nivelConfig.tipoPersonaRechazar = extraerTextoDeCorchetes(regla);
                break;
            case 2:
            {
                std::vector<std::string> valores = extraerVariosTextosCorchetes(regla);
                if (valores.size() >= 2) {
                    nivelConfig.tipoPersonaRechazar = valores[0];
                    nivelConfig.motivoViajeRestringido = valores[1];
                }
            }
            break;
            case 3:
            {
                std::vector<std::string> valores = extraerVariosTextosCorchetes(regla);
                if (valores.size() >= 2) {
                    nivelConfig.nacionalidadStruct = valores[0];
                    nivelConfig.residenciaStruct = valores[1];
                }
            }
            break;
            case 4:
                nivelConfig.maxFallosPermitidos = extraerValorDeCorchetes(regla);
                break;
            case 5:
            {
                std::vector<std::string> paises = extraerVariosTextosCorchetes(regla);
                nivelConfig.paisesEnGuerra[0]=paises[0];paisesGuerra.push_back(paises[0]);
                nivelConfig.paisesEnGuerra[1]=paises[1];paisesGuerra.push_back(paises[1]);
                nivelConfig.paisesEnGuerra[2]=paises[2];paisesGuerra.push_back(paises[2]);
                nivelConfig.paisesEnGuerra[3]=paises[3];paisesGuerra.push_back(paises[3]);
            }
            break;
            }
            break;
        }
        case 5: { //NIVEL 5
            switch (linea) {
            case 0:
                nivelConfig.cantidadPersonas = extraerValorDeCorchetes(regla);
                break;
            case 1:
            {
                std::vector<std::string> valores = extraerVariosTextosCorchetes(regla);
                if (valores.size() >= 2) {
                    nivelConfig.tipoVisitaRestringido = valores[0];
                    nivelConfig.duracionMaximaEstancia = stoi(valores[1]);
                }
            }
            break;
            case 2:
                nivelConfig.tipoPersonaRechazar = extraerTextoDeCorchetes(regla);
                break;
            case 3:
            {
                std::vector<std::string> valores = extraerVariosTextosCorchetes(regla);
                if (valores.size() >= 2) {
                    nivelConfig.tipoPersonaRechazar = valores[0];
                    nivelConfig.motivoViajeRestringido = valores[1];
                }
            }
            break;
            case 4:
            {
                std::vector<std::string> valores = extraerVariosTextosCorchetes(regla);
                if (valores.size() >= 2) {
                    nivelConfig.nacionalidadStruct = valores[0];
                    nivelConfig.residenciaStruct = valores[1];
                }
            }
            break;
            case 5:
            {
                std::vector<std::string> valores = extraerVariosTextosCorchetes(regla);
                if (valores.size() >= 2) {
                    nivelConfig.duracionMaximaEstancia = std::stoi(valores[0]);
                    nivelConfig.motivoViajeRestringido = valores[1];
                }
            }
            break;
            case 6:
                nivelConfig.estadoAmor = extraerValorDeCorchetes(regla);
                break;
            case 7:
                 nivelConfig.edad = extraerValorDeCorchetes(regla);
            break;
            case 8:
            {
                std::vector<std::string> valores = extraerVariosTextosCorchetes(regla);
                if (valores.size() >= 2) {
                    nivelConfig.edad = std::stoi(valores[0]);
                    nivelConfig.motivoViajeRestringido = valores[1];
                }
            }
            break;
            case 9:
                nivelConfig.maxFallosPermitidos =extraerValorDeCorchetes(regla);
                break;




            }
            break;
        }
        default:
            QMessageBox::critical(mainWindow, "Error", QString("Nivel %1 no está definido.").arg(nivel));
            QApplication::quit();
        }
        acumularReglas += reglaQt + "\n";
        linea++;
    }

    archivoReglas.close();
}

void Juego::cargarPersonasNivel(int nivel) {
    personas.clear();
    nombresUsados.clear();
    imagenesUsadas.clear();
    int cantidadPersonas=nivelConfig.cantidadPersonas;

    // Mezclar las listas de nombres y imágenes
    std::shuffle(nombresHombres.begin(), nombresHombres.end(), rng);
    std::shuffle(nombresMujeres.begin(), nombresMujeres.end(), rng);
    std::shuffle(imagenesHombres.begin(), imagenesHombres.end(), rng);
    std::shuffle(imagenesMujeres.begin(), imagenesMujeres.end(), rng);

    size_t nombreIndexH = 0;
    size_t nombreIndexM = 0;
    size_t imagenIndexH = 0;
    size_t imagenIndexM = 0;

    std::uniform_int_distribution<int> distTipoPersona(0, 3); //valores aleatorios
    std::uniform_int_distribution<int> distBooleano(0, 1);

    std::uniform_int_distribution<size_t> distPais(0, listaPaisesAmerica.size() - 1);
    std::uniform_int_distribution<size_t> distTipoV(0, listaTiposVisita.size() - 1);
    std::uniform_int_distribution<size_t> distECivil(0, listaEstadosCiviles.size() - 1);
    std::uniform_int_distribution<size_t> distMotivo(0, listaMotivosViaje.size() - 1);
    std::uniform_int_distribution<int> distDuracion(1, 10);
    std::uniform_int_distribution<size_t> distTipoPersonaIndex(0, 3);

    vector<string> tiposPersona = {"ALDEANO", "REFUGIADO", "DIPLOMATICO", "REVOLUCIONARIO"};

    for (int i = 0; i < cantidadPersonas; ++i) {
        Persona p;
        // Asignar género aleatoriamente
        bool generoAleatorio = (distBooleano(rng) == 0); // true: hombre, false: mujer
        p.setGenero(generoAleatorio);

        // Seleccionar nombre
        std::string nombreSeleccionado;
        if (generoAleatorio) { // Hombre
            if (nombreIndexH >= nombresHombres.size()) {
                QMessageBox::critical(mainWindow, "Error", "No quedan nombres de hombres disponibles.");
                break;
            }
            nombreSeleccionado = nombresHombres[nombreIndexH++];
        } else { // Mujer
            if (nombreIndexM >= nombresMujeres.size()) {
                QMessageBox::critical(mainWindow, "Error", "No quedan nombres de mujeres disponibles.");
                break;
            }
            nombreSeleccionado = nombresMujeres[nombreIndexM++];
        }
        p.setNombreApellido(nombreSeleccionado);

        // Seleccionar imagen
        QString imagenSeleccionada;
        if (generoAleatorio) { // Hombre
            if (imagenIndexH >= imagenesHombres.size()) {
                QMessageBox::critical(mainWindow, "Error", "No quedan imágenes de hombres disponibles.");
                break;
            }
            imagenSeleccionada = imagenesHombres[imagenIndexH++];
        } else { // Mujer
            if (imagenIndexM >= imagenesMujeres.size()) {
                QMessageBox::critical(mainWindow, "Error", "No quedan imágenes de mujeres disponibles.");
                break;
            }
            imagenSeleccionada = imagenesMujeres[imagenIndexM++];
        }
        p.setRutaImagen(imagenSeleccionada);

        // Asignar nacionalidad y país de residencia aleatoriamente
        string nacionalidad = listaPaisesAmerica[distPais(rng)];
        string paisResi = listaPaisesAmerica[distPais(rng)];

        p.setNacionalidad(nacionalidad);
        p.setPaisResi(paisResi);
        // Asignar fecha de nacimiento y calcular la edad
        p.setFechaNac(generarFechaAleatoria());
        int edad = calcularEdad(p.obtenerFechaNac());

        // Asignar otros atributos aleatoriamente
        p.setFechaNac(generarFechaAleatoria());
        p.setTipoV(listaTiposVisita[distTipoV(rng)]);
        p.setMotivo(listaMotivosViaje[distMotivo(rng)]);
        p.setDuracion(distDuracion(rng)); // Duración entre 1 y 10 semanas
        p.setViajaSolo(distBooleano(rng) == 0);
        p.setTipoPersona(tiposPersona[distTipoPersonaIndex(rng)]);
        p.setDocumentacionValida(distBooleano(rng) == 0);
        // Asignar estado civil según la edad
        if (edad < 18) {
            p.setECivil("Soltero");  // Menores de edad solo pueden estar "Soltero"
        } else {
            p.setECivil(listaEstadosCiviles[distECivil(rng)]); // Asignación aleatoria para mayores de edad
        }

        for (auto pa : paisesGuerra) {
            if (nacionalidad==pa){
                p.setPaisRestringido(true);
            }else{
                p.setPaisRestringido(false);
            }
        }
        //p.setPaisRestringido(paisesGuerra.find(nacionalidad) != paisesgu.end());

        personas.push_back(p);
    }
}

void Juego::mostrarPersonaActual() {
    if (indicePersonaActual < personas.size()) {
        const Persona& p = personas[indicePersonaActual];
        verificacionesRestantes = nivelConfig.maxVerificaciones;
        int edad = calcularEdad(p.obtenerFechaNac());
        QString info = QString("Nombre y Apellido: %1\n").arg(QString::fromStdString(p.obtenerNombreApellido()));
        info += QString("Género: %1\n").arg(p.getGenero() ? "Hombre" : "Mujer");
        info += QString("Nacionalidad: %1\nFecha de Nacimiento: %2\nEdad: %3 años\nTipo de Persona: %4\nTipo de Visita: %5\nDuración: %6 semanas\nEstado Civil: %7")
                    .arg(QString::fromStdString(p.obtenerNacionalidad()))
                    .arg(QString::fromStdString(p.obtenerFechaNac()))
                    .arg(edad)
                    .arg(QString::fromStdString(p.getTipoPersona()))
                    .arg(QString::fromStdString(p.obtenerTipoV()))
                    .arg(p.obtenerDuracion())
                    .arg(QString::fromStdString(p.obtenerECivil()));
        if (nivelActual >= 2) {
            info += QString("\nPaís de Residencia: %1\nMotivo del Viaje: %2")
                        .arg(QString::fromStdString(p.obtenerPaisResi()))
                        .arg(QString::fromStdString(p.obtenerMotivo()));
        }
        if (nivelActual >= 3) {
            info += QString("\nViaja Solo: %1").arg(p.obtenerViajaSolo() ? "Sí" : "No");
        }
        if (nivelActual >= 4) {
            info += QString("\nVisita país restringido [VERIFICAR]");
            info += QString("\nDocumentación válida [VERIFICAR]");
            verificacionesRestantes = nivelConfig.maxVerificaciones;
        }

        personaInfo->setText(info);
        nivelLabel->setText("Nivel: " + QString::number(nivelActual));
        puntosLabel->setText("Puntos: " + QString::number(control.getPuntos()));
        erroresLabel->setText("Errores: " + QString::number(errores));

        // Mostrar imagen
        QPixmap pixmap(p.getRutaImagen());
        if (!pixmap.isNull()) {
            // Escalar el pixmap al tamaño fijo del QLabel manteniendo las proporciones
            QPixmap scaledPixmap = pixmap.scaled(imagenPersonaLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            imagenPersonaLabel->setPixmap(scaledPixmap);
        } else {
            imagenPersonaLabel->setText("Imagen no disponible");
        }

        // Mostrar botones de verificación
        verificarPaisRestringidoButton->setVisible(nivelActual >= 3);
        verificarDocumentacionButton->setVisible(nivelActual >= 3);
    } else {
        finJuego("Has completado todos los niveles.");
    }
}

void Juego::mostrarRestricciones() {
    QMessageBox::information(mainWindow, "Reglas del Nivel " + QString::number(nivelActual), acumularReglas);
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

int Juego::calcularEdad(const string& fechaNac) {
    int anioNacimiento, mesNacimiento, diaNacimiento;
    sscanf(fechaNac.c_str(), "%d-%d-%d", &anioNacimiento, &mesNacimiento, &diaNacimiento);

    // Obtener la fecha actual
    time_t t = time(nullptr);
    struct tm* now = localtime(&t);
    int anioActual = now->tm_year + 1900;
    int mesActual = now->tm_mon + 1;
    int diaActual = now->tm_mday;
    int edad = anioActual - anioNacimiento;
    if (mesActual < mesNacimiento || (mesActual == mesNacimiento && diaActual < diaNacimiento)) {
        edad--;
    }
    return edad;
}

void Juego::cargarProgreso() {
    nivelActual = 1;
    control.setPuntos(0);
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

void Juego::finJuego(const std::string& mensaje) {
    sonidoAmbiente.stop();
    QMessageBox::information(mainWindow, "Fin del Juego", QString::fromStdString(mensaje));
    QApplication::quit();
}
bool Juego::evaluarDecision(const Persona& p, bool decision) {
    if (p.getTipoPersona() == "REVOLUCIONARIO") {
        if (decision) {
            return false;
        } else {
            return true;
        }
    }
    if (nivelActual == 1) {  // Nivel 1
        if (decision && p.obtenerTipoV() == nivelConfig.tipoVisitaRestringido)
            return false;
        if (decision && p.obtenerDuracion() > nivelConfig.duracionMaximaEstancia)
            return false;

        if (!decision && p.obtenerTipoV() != nivelConfig.tipoVisitaRestringido &&
            p.obtenerDuracion() <= nivelConfig.duracionMaximaEstancia)
            return false;  // debio ser aceptado
        return true;
    }

    if (nivelActual == 2) {  // Nivel 2
        if (decision && p.obtenerTipoV() == nivelConfig.tipoVisitaRestringido &&
            p.obtenerDuracion() > nivelConfig.duracionMaximaEstancia)
            return false;
        if (decision && p.obtenerNacionalidad() == nivelConfig.paisRechazar)
            return false;
        if (decision && p.obtenerMotivo() == nivelConfig.motivoViajeRestringido)
            return false;

        if (!decision && !(p.obtenerTipoV() == nivelConfig.tipoVisitaRestringido &&
                           p.obtenerDuracion() > nivelConfig.duracionMaximaEstancia) &&
            p.obtenerNacionalidad() != nivelConfig.paisRechazar &&
            p.obtenerMotivo() != nivelConfig.motivoViajeRestringido)
            return false;  // debio ser aceptado
        return true;
    }

    if (nivelActual == 3) {  // Nivel 3
        if (decision && calcularEdad(p.obtenerFechaNac()) < 18 && p.obtenerViajaSolo())
            return false;
        if (decision && p.getTipoPersona() == nivelConfig.tipoPersonaRechazar && p.obtenerViajaSolo())
            return false;
        if (decision && p.obtenerTipoV() == nivelConfig.tipoVisitaRestringido &&
            p.obtenerDuracion() < nivelConfig.duracionMaximaEstancia)
            return false;

        if (!decision && !(calcularEdad(p.obtenerFechaNac()) < 18 && p.obtenerViajaSolo()) &&
            !(p.getTipoPersona() == nivelConfig.tipoPersonaRechazar && p.obtenerViajaSolo()) &&
            !(p.obtenerTipoV() == nivelConfig.tipoVisitaRestringido &&
              p.obtenerDuracion() < nivelConfig.duracionMaximaEstancia))
            return false;  // debio ser aceptado
        return true;
    }

    if (nivelActual == 4) {  // Nivel 4
        if (decision && p.getTipoPersona() == nivelConfig.tipoPersonaRechazar && !p.obtenerDocumentacionValida())
            return false;
        if (decision && p.getTipoPersona() == nivelConfig.tipoPersonaRechazar &&
            p.obtenerMotivo() == nivelConfig.motivoViajeRestringido)
            return false;
        for (const auto& pais : paisesGuerra) {
            if (decision && (p.obtenerNacionalidad() == pais || p.obtenerPaisResi() == pais))
                return false;
        }

        if (!decision && p.getTipoPersona() != nivelConfig.tipoPersonaRechazar && p.obtenerDocumentacionValida() &&
            p.getTipoPersona() != nivelConfig.tipoPersonaRechazar &&
            p.obtenerMotivo() != nivelConfig.motivoViajeRestringido) {
            bool inWarCountry = false;
            for (const auto& pais : paisesGuerra) {
                if (p.obtenerNacionalidad() == pais || p.obtenerPaisResi() == pais) {
                    inWarCountry = true;
                    break;
                }
            }
            if (!inWarCountry)
                return false;  // debio ser aceptado
        }
        return true;
    }
    if (nivelActual == 5) {  // Nivel 5
        if (decision && p.obtenerTipoV() == nivelConfig.tipoVisitaRestringido &&
            p.obtenerDuracion() > nivelConfig.duracionMaximaEstancia)
            return false;
        if (decision && p.getTipoPersona() == nivelConfig.tipoPersonaRechazar && !p.obtenerDocumentacionValida())
            return false;
        if (decision && p.getTipoPersona() == nivelConfig.tipoPersonaRechazar &&
            p.obtenerMotivo() == nivelConfig.motivoViajeRestringido)
            return false;
        for (const auto& paisGuerra : paisesGuerra) {
            if (decision && (p.obtenerNacionalidad() == paisGuerra || p.obtenerPaisResi() == paisGuerra))
                return false;
        }
        if (decision && p.obtenerDuracion() < nivelConfig.duracionMaximaEstancia &&
            p.obtenerMotivo() == nivelConfig.motivoViajeRestringido)
            return false;
        if (decision && p.obtenerECivil() == nivelConfig.estadoAmor && p.obtenerViajaSolo())
            return false;
        if (decision && calcularEdad(p.obtenerFechaNac()) < nivelConfig.edad && p.obtenerViajaSolo())
            return false;
        if (decision && calcularEdad(p.obtenerFechaNac()) > 40 && p.obtenerMotivo() == nivelConfig.motivoViajeRestringido)
            return false;

        if (!decision && !(p.obtenerTipoV() == nivelConfig.tipoVisitaRestringido &&
                           p.obtenerDuracion() > nivelConfig.duracionMaximaEstancia) &&
            p.getTipoPersona() != nivelConfig.tipoPersonaRechazar && p.obtenerDocumentacionValida()) {
            bool inWarCountry = false;
            for (const auto& pais : paisesGuerra) {
                if (p.obtenerNacionalidad() == pais || p.obtenerPaisResi() == pais) {
                    inWarCountry = true; //aca basicamente evaluo los paises en guerra
                    break;
                }
            }
            if (!inWarCountry && !(p.obtenerDuracion() < nivelConfig.duracionMaximaEstancia &&
                                   p.obtenerMotivo() == nivelConfig.motivoViajeRestringido))
                return false;  // se debio aceptar
        }
        return true;
    }
    return true;  // Si no hay condiciones de falla
}
void Juego::aceptarPersona() {
    if (indicePersonaActual < personas.size()) {
        const Persona& p = personas[indicePersonaActual];
        bool decisionCorrecta = evaluarDecision(p, true);
        int error;
        if (decisionCorrecta) {
            control.incrementarPuntos(puntosPorTipo[p.getTipoPersona()]);
            resultadoLabel->setText("Aceptaste correctamente a la persona.");
            sonidoCorrecto.play();
            // Registrar la decisión correcta
            logger.log(QString("Aceptar: %1 (%2). Correcto.").arg(QString::fromStdString(p.obtenerNombreApellido())).arg(QString::fromStdString(p.getTipoPersona())));
        } else {
            control.incrementarPuntos(-puntosPorTipo[p.getTipoPersona()]);
            errores++;
            resultadoLabel->setText("Debiste rechazar a esta persona.");
            sonidoError.play();
            // Registrar la decisión incorrecta
            logger.log(QString("Aceptar: %1 (%2). Incorrecto: debería haber rechazado.").arg(QString::fromStdString(p.obtenerNombreApellido())).arg(QString::fromStdString(p.getTipoPersona())));
        }
        error+=errores;
        puntosLabel->setText("Puntos: " + QString::number(control.getPuntos()));
        erroresLabel->setText("Errores: " + QString::number(error));

        indicePersonaActual++;
        personasProcesadasNivel++;

        if (errores >= nivelConfig.maxFallosPermitidos) {
            finJuego("Has cometido demasiados errores. Fin del juego.");
            return;
        }

        if (personasProcesadasNivel >= nivelConfig.cantidadPersonas) {
            nivelActual++;
            if (nivelActual > 5) {
                finJuego("Has completado todos los niveles.");
                return;
            } else {
                cargarConfiguracionNivel(nivelActual);
                cargarPersonasNivel(nivelActual);
                personasProcesadasNivel = 0;
                indicePersonaActual = 0;
                errores=0;
                QMessageBox::information(mainWindow, "Nivel Completado", "Has pasado al nivel " + QString::number(nivelActual));
                // Registrar el cambio de nivel
                logger.log(QString("Nivel %1 completado. Pasando al nivel %2.").arg(nivelActual - 1).arg(nivelActual));
                mostrarRestricciones();
            }
        }

        mostrarPersonaActual();
    }
}

void Juego::rechazarPersona() {
    if (indicePersonaActual < personas.size()) {
        const Persona& p = personas[indicePersonaActual];
        bool decisionCorrecta = evaluarDecision(p, false);
        int err;
        if (decisionCorrecta) {
            control.incrementarPuntos(puntosPorTipo[p.getTipoPersona()]);
            resultadoLabel->setText("Rechazaste correctamente a la persona.");
            sonidoCorrecto.play();
            // registrar la decisión correcta
            logger.log(QString("Rechazar: %1 (%2). Correcto.").arg(QString::fromStdString(p.obtenerNombreApellido())).arg(QString::fromStdString(p.getTipoPersona())));
        } else {
            control.incrementarPuntos(-puntosPorTipo[p.getTipoPersona()]);
            errores++;
            resultadoLabel->setText("Debiste aceptar a esta persona.");
            sonidoError.play();
            // registrar la decisión incorrecta
            logger.log(QString("Rechazar: %1 (%2). Incorrecto: debería haber aceptado.").arg(QString::fromStdString(p.obtenerNombreApellido())).arg(QString::fromStdString(p.getTipoPersona())));
        }
        err+=errores;
        puntosLabel->setText("Puntos: " + QString::number(control.getPuntos()));
        erroresLabel->setText("Errores: " + QString::number(err));

        indicePersonaActual++;
        personasProcesadasNivel++;

        if (errores >= nivelConfig.maxFallosPermitidos) {
            finJuego("Has cometido demasiados errores. Fin del juego.");
            return;
        }

        if (personasProcesadasNivel >= nivelConfig.cantidadPersonas) {
            nivelActual++;
            if (nivelActual > obtenerNumeroNiveles()) {
                finJuego("Has completado todos los niveles.");
                return;
            } else {
                cargarConfiguracionNivel(nivelActual);
                cargarPersonasNivel(nivelActual);
                personasProcesadasNivel = 0;
                indicePersonaActual = 0;
                errores=0;
                QMessageBox::information(mainWindow, "Nivel Completado", "Has pasado al nivel " + QString::number(nivelActual));
                // Registrar el cambio de nivel
                logger.log(QString("Nivel %1 completado. Pasando al nivel %2.").arg(nivelActual - 1).arg(nivelActual));
                mostrarRestricciones();
            }
        }
        mostrarPersonaActual();
    }
}

void Juego::verificarPaisRestringido() {
    if (nivelActual >= 3 && verificacionesRestantes > 0) {
        const Persona& p = personas[indicePersonaActual];
        verificacionesRestantes--;

        if (p.obtenerPaisRestringido()) {
            resultadoLabel->setText("La persona viene de un país restringido.");
            // Registrar la verificación
            logger.log(QString("Verificación: %1 viene de un país restringido.").arg(QString::fromStdString(p.obtenerNombreApellido())));
        } else {
            resultadoLabel->setText("La persona no viene de un país restringido.");
            // Registrar la verificación
            logger.log(QString("Verificación: %1 no viene de un país restringido.").arg(QString::fromStdString(p.obtenerNombreApellido())));
        }
    } else {
        resultadoLabel->setText("No tienes más verificaciones disponibles.");
    }
}

void Juego::verificarDocumentacion() {
    if (nivelActual >= 3 && verificacionesRestantes > 0) {
        const Persona& p = personas[indicePersonaActual];
        verificacionesRestantes--;

        if (p.obtenerDocumentacionValida()) {
            resultadoLabel->setText("La documentación es válida.");
            // registrar la verificación
            logger.log(QString("Verificación: %1 tiene documentación válida.").arg(QString::fromStdString(p.obtenerNombreApellido())));
        } else {
            resultadoLabel->setText("La documentación no es válida.");
            // registrar la verificación
            logger.log(QString("Verificación: %1 tiene documentación inválida.").arg(QString::fromStdString(p.obtenerNombreApellido())));
        }
    } else {
        resultadoLabel->setText("No tienes más verificaciones disponibles.");
    }
}
int Juego::obtenerNumeroNiveles() {
    return 5; //
}

// Métodos de serialización
void Juego::saveGame(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(mainWindow, "Guardar Partida", "No se pudo abrir el archivo para guardar.");
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15); // Asegurar compatibilidad

    // Guardar ControlFronterizo
    control.serialize(out);

    // Guardar NivelActual, Errores, IndicePersonaActual, PersonasProcesadasNivel, VerificacionesRestantes
    out << nivelActual << errores << indicePersonaActual << personasProcesadasNivel << verificacionesRestantes;

    // Guardar restricciones acumuladas
    out << static_cast<quint32>(restriccionesAcumuladas.size());
    for (const auto& restriccion : restriccionesAcumuladas) {
        out << QString::fromStdString(restriccion);
    }

    // Guardar nivelConfig
    nivelConfig.serialize(out);

    // Guardar personas
    out << static_cast<quint32>(personas.size());
    for (const auto& persona : personas) {
        persona.serialize(out);
    }

    file.close();
    QMessageBox::information(mainWindow, "Guardar Partida", "La partida se ha guardado exitosamente.");
    // Registrar el guardado de la partida
    logger.log(QString("Partida guardada en %1.").arg(filename));
}
void Juego::loadGame(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(mainWindow, "Cargar Partida", "No se pudo abrir el archivo para cargar.");
        return;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_15);
    // Cargar ControlFronterizo
    control.deserialize(in);

    // Cargar NivelActual, Errores, IndicePersonaActual, PersonasProcesadasNivel, VerificacionesRestantes
    in >> nivelActual >> errores >> indicePersonaActual >> personasProcesadasNivel >> verificacionesRestantes;

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
    puntosLabel->setText("Puntos: " + QString::number(control.getPuntos()));
    erroresLabel->setText("Errores: " + QString::number(errores));

    mostrarPersonaActual();

    QMessageBox::information(mainWindow, "Cargar Partida", "La partida se ha cargado exitosamente.");
    // Registrar la carga de la partida
    logger.log(QString("Partida cargada desde %1.").arg(filename));
}

bool Juego::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
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

    QVBoxLayout* layoutDialog = new QVBoxLayout(&dialog);

    QPushButton* guardarPartidaBtn = new QPushButton("Guardar Partida");
    QPushButton* volverMenuBtn = new QPushButton("Volver al Menú Principal");
    QPushButton* cerrarJuegoBtn = new QPushButton("Cerrar Juego");

    // Estilizar botones
    QString botonMenuEstilo =
        "QPushButton {"
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
    connect(guardarPartidaBtn, &QPushButton::clicked, this, &Juego::guardarPartida);
    connect(volverMenuBtn, &QPushButton::clicked, [&]() {
        dialog.accept();
        volverAlMenu();
    });
    connect(cerrarJuegoBtn, &QPushButton::clicked, this, &Juego::cerrarJuego);

    // Mostrar el diálogo
    dialog.exec();
}
void Juego::guardarPartida() {
    QString filename = QFileDialog::getSaveFileName(mainWindow, "Guardar Partida", "", "Partida Guardada (*.gam)");
    if (!filename.isEmpty()) {
        if (!filename.endsWith(".gam")) {
            filename += ".gam";
        }
        saveGame(filename);
    }
}

void Juego::cerrarJuego() {
    QApplication::quit();
}

void Juego::cargarPartida() {
    QString filename = QFileDialog::getOpenFileName(mainWindow, "Cargar Partida", "", "Partida Guardada (*.gam)");
    if (!filename.isEmpty()) {
        loadGame(filename);
        stackedWidget->setCurrentIndex(2); // Cambiar a la pantalla de juego
    }
}
#include "main.moc"
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("myGatekeeper");
    mainWindow.setFixedSize(1024, 768); // Resolucion
    Juego juego(&mainWindow);
    mainWindow.show();
    return app.exec();
}
