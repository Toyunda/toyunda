#include <QApplication>
#include <QTextStream>
#include "sqarg.h"
#include "qtoyunda.h"
#ifdef Q_WS_X11
#include <X11/extensions/Xrender.h>
#endif

void	defineOption(SQArgDescMap &);

int	main(int ac, char *ag[])
{
  QToyunda              *toyunda;
  QApplication		*qapp;
  SQArgDescMap		optionDesc;
  QMap<QString, QVariant> option;

  defineOption(optionDesc);
  qapp = new QApplication(ac, ag);
  // Handle arguments
  QStringList arg = qapp->arguments();
  arg.removeFirst();
  bool vopt = SQArg::fillWithDesc(option, arg, optionDesc);
  // Help
  if (option["help"].toBool() or vopt == false) {
    QTextStream cout(stdout);
    cout << "QToyunda\nSynopsis : ";
    cout << "./qtoyunda --player playername --renderer renderername --video videofile --subtitle subtitlefile\n\n";
    cout.flush();
    if (option["help"].toBool()) {
      cout << "All option available :\n";
      cout.flush();
      SQArg::generateLongHelp(optionDesc);
      return 0;
    }
    return 1;
  }
  // Player and renderer option
  QStringList playerOption = option["playeroption"].toString().split(",");
  QStringList rendererOption = option["rendereroption"].toString().split(",");

  toyunda = new QToyunda(option["player"].toString(), option["renderer"].toString(), playerOption, rendererOption);

  // FIXME playeroptionhelp and roh must not depend and toyunda
  if (option["playeroptionhelp"].toBool()) {
    toyunda->showPlayerOption();
    return 0;
  }
  if (option["rendereroptionhelp"].toBool()) {
    toyunda->showRendererOption();
    return 0;
  }
  qDebug() << "Init toyunda";
  toyunda->init();
  qDebug() << "Load files";
  toyunda->load(option["video"].toString(), option["subtitle"].toString());
  qDebug() << "Play the video";
  toyunda->play();
  toyunda->showRenderer();
  return qapp->exec();
}

void	defineOption(SQArgDescMap &optionDesc)
{
  // FIXME rendereroptionhelp and playeroptionhelp must take the name
  optionDesc["help"] = SQOpt("help", "h", false, "Show this help", "Show this help", false, true);
  optionDesc["renderer"] = SQOpt("renderer", "r", QString(""),
   "Set the renderer", "Set the renderer, use --rendererlist to have the list", true);
  optionDesc["player"] = SQOpt("player", "p", QString(""),
    "Set the player", "Set the player, use --playerlist to have the list", true);

  optionDesc["rendererlist"] = SQOpt("rendererlist", "rl", false,
    "Show all available renderer", "Show all available renderer", false, true);
  optionDesc["rendereroptionhelp"] = SQOpt("rendereroptionhelp", "roh", false,
    "Show option of the renderer", "Show option of the renderer, YOU must set --renderer", false, true);
  optionDesc["playerlist"] = SQOpt("playerlist", "pl", false,
    "Show all available player", "Show all available player", false, true);
  optionDesc["playeroptionhelp"] = SQOpt("playeroptionhelp", "poh", false,
    "Show option of the player", "Show option of the player, YOU must set --player", false, true);

  optionDesc["rendereroption"] = SQOpt("rendereroption", "ro", QString(""),
    "Set the renderer option", "Set the renderer's options, format : opt1=value1,opt2=value2", false);
  optionDesc["playeroption"] = SQOpt("playeroption", "po", QString(""), "Set the player option",
    "Set the player's options, format : opt1=value1,opt2=value2", false);
  optionDesc["subtitle"] = SQOpt("subtitle", "sub", QString("none"), "Set the subtitle file",
    "Set the subtitle file", true);
  optionDesc["video"] = SQOpt("video", "vid", QString("none"), "Set the video file",
    "Set the video file", true);
}
