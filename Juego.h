#ifndef JUEGO_H
#define JUEGO_H

#include <QApplication>
#include <QDataStream>
#include <QFile>
#include <QFileDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QSoundEffect>
#include <QStackedWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


#include "ControlFronterizo.h"
#include "LevelConfig.h"
#include "Logger.h"
#include "Persona.h"


using namespace std;

class Juego : public QObject {
  Q_OBJECT
private:
  ControlFronterizo control;
  vector<Persona> personas;
  vector<string> paisesGuerra;
  int nivelActual = 1;
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
  QMainWindow *mainWindow;
  QLabel *resultadoLabel;
  QLabel *nivelLabel;
  QLabel *puntosLabel;
  QLabel *erroresLabel;
  QLabel *imagenPersonaLabel;
  QTextEdit *personaInfo;
  QPushButton *aceptarButton;
  QPushButton *rechazarButton;
  QPushButton *verificarPaisRestringidoButton;
  QPushButton *verificarDocumentacionButton;
  QPushButton *verRestriccionesButton; // **Nuevo botón para restricciones**
  QPushButton *guardarButton;          // Botón de guardar en el menú in-game
  QPushButton *volverMenuButton;       // Botón para volver al menú principal
  QPushButton *cerrarJuegoButton;      // Botón para cerrar el juego
  QVBoxLayout *layout;
  // Menú de inicio y reglas
  QWidget *menuInicioWidget;
  QWidget *reglasWidget;
  QPushButton *jugarButton;
  QPushButton *reglasButton;
  QPushButton *salirButton;
  QPushButton *cargarPartidaButton; // **Nuevo botón para cargar partida**
  QPushButton *volverButton;
  QStackedWidget *stackedWidget;
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
  Juego(QMainWindow *mw);

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
  int calcularEdad(const string &fechaNac);
  void cargarProgreso();
  void finJuego(const std::string &mensaje);
  bool evaluarDecision(const Persona &p, bool decision);
  int obtenerNumeroNiveles();
  // Métodos de serialización
  void saveGame(const QString &filename);
  void loadGame(const QString &filename);

protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
  void aceptarPersona();
  void rechazarPersona();
  void verificarPaisRestringido();
  void verificarDocumentacion();
  void mostrarRestricciones(); // **Nuevo slot para mostrar restricciones**
  void iniciarJuego();         // **Declaración agregada**
  void showInGameMenu();       // **Nuevo slot para mostrar menú in-game**
  void guardarPartida();       // **Nuevo slot para guardar partida**
  void cerrarJuego();          // **Nuevo slot para cerrar juego**
  void cargarPartida();        // **Nuevo slot para cargar partida**
};

#endif // JUEGO_H
