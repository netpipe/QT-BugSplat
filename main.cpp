#include <QApplication>
#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QSoundEffect>
#include <QMenuBar>
#include <QSettings>
#include <QScreen>
#include <QFile>
#include <QTimer>

class BugSplatGame : public QMainWindow {
    Q_OBJECT

public:
    BugSplatGame() {
        setWindowTitle("Bug Splat");

        loadSettings();
        setupMenu();
       // setupAudio();

        splat = QPixmap("fly.png");
        splatTimer = new QTimer(this);
        splatTimer->setSingleShot(true);
        connect(splatTimer, &QTimer::timeout, this, [&]() {
            showSplat = true;
            update();
        });


        splatsound.setSource(QUrl("qrc:/splat.wav"));
        music.setSource(QUrl("qrc:/music.wav"));
music.play();

        bug = QPixmap(":/fly.png");
        hand = QPixmap(":/hand.png");

        bugPos = QPoint(rand() % width(), rand() % height());

        QTimer* timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [&]() {
            update();
        });
        timer->start(16); // ~60 FPS
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.drawPixmap(bugPos, bug);
        if (showHand)
            p.drawPixmap(handPos, hand);
        if (showSplat)
            p.drawPixmap(splatPos, splat);
    }

    void mousePressEvent(QMouseEvent* event) override {
        handPos = event->pos();
        showHand = true;
        QTimer::singleShot(200, this, [&]() { showHand = false; update(); });

        QRect bugRect(bugPos, bug.size());      
        if (bugRect.contains(event->pos())) {
            splatPos = bugPos;
            showSplat = true;
            splatTimer->start(500); // remove splat after 0.5 seconds
            if (soundOn && splatsound.Ready)
                splatsound.play();
            bugPos = QPoint(rand() % (width() - bug.width()), rand() % (height() - bug.height()));
        }

        update();
    }

    void closeEvent(QCloseEvent*) override {
        saveSettings();
    }

private:
    QPixmap bug, hand;
    QPoint bugPos, handPos;
    bool showHand = true;
    bool soundOn = true;
    bool fullscreen = false;
    QSize screenSize = QSize(800, 600);

    QPixmap splat;
    QPoint splatPos;
    bool showSplat = false;
    QTimer* splatTimer;

 QSoundEffect splatsound, music;

    void setupMenu() {
        QMenu* options = menuBar()->addMenu("Options");

        QAction* toggleSound = options->addAction("Toggle Sound");
        connect(toggleSound, &QAction::triggered, [&]() {
            soundOn = !soundOn;
           // if (soundOn) player->play();
          //  else player->stop();
        });

        QAction* toggleFullscreen = options->addAction("Toggle Fullscreen");
        connect(toggleFullscreen, &QAction::triggered, [&]() {
            fullscreen = !fullscreen;
            if (fullscreen)
                showFullScreen();
            else
                showNormal();
        });

        QAction* setSmall = options->addAction("800x600");
        connect(setSmall, &QAction::triggered, [&]() {
            screenSize = QSize(800, 600);
            resize(screenSize);
        });

        QAction* setLarge = options->addAction("1280x720");
        connect(setLarge, &QAction::triggered, [&]() {
            screenSize = QSize(1280, 720);
            resize(screenSize);
        });
    }

    void setupAudio() {
        //player = new QMediaPlayer(this);
        //if (QFile::exists(":/music.wav")) {
            //player->setMedia(QUrl::fromLocalFile(":/music.wav"));
       //     player->setMedia(QUrl(":/music.wav"));
      //      player->setVolume(100);
           // if (soundOn)
                music.play();
       // }
    }

    void loadSettings() {
        QSettings settings("MyCompany", "BugSplatGame");
        soundOn = settings.value("soundOn", true).toBool();
        fullscreen = settings.value("fullscreen", false).toBool();
        screenSize = settings.value("screenSize", QSize(800, 600)).toSize();

        resize(screenSize);
        if (fullscreen)
            showFullScreen();
    }

    void saveSettings() {
        QSettings settings("MyCompany", "BugSplatGame");
        settings.setValue("soundOn", soundOn);
        settings.setValue("fullscreen", fullscreen);
        settings.setValue("screenSize", screenSize);
    }
};



int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    BugSplatGame game;
    game.show();
    return a.exec();
}
#include "main.moc"
