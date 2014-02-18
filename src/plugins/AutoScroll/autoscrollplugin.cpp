/* ============================================================
* QupZilla - WebKit based browser
* Copyright (C) 2014  David Rosca <nowrep@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#include "autoscrollplugin.h"
#include "autoscroller.h"
#include "qupzilla.h"
#include "pluginproxy.h"
#include "mainapplication.h"

#include <QTranslator>

AutoScrollPlugin::AutoScrollPlugin()
    : QObject()
    , m_scroller(0)
{
}

PluginSpec AutoScrollPlugin::pluginSpec()
{
    PluginSpec spec;
    spec.name = "AutoScroll";
    spec.info = "AutoScroll plugin";
    spec.description = "Provides support for autoscroll with middle mouse button";
    spec.version = "0.1.1";
    spec.author = "David Rosca <nowrep@gmail.com>";
    spec.icon = QPixmap(":/autoscroll/data/icon.png");
    spec.hasSettings = false;

    return spec;
}

void AutoScrollPlugin::init(InitState state, const QString &settingsPath)
{
    Q_UNUSED(state)

    m_scroller = new AutoScroller(settingsPath, this);

    QZ_REGISTER_EVENT_HANDLER(PluginProxy::MouseMoveHandler);
    QZ_REGISTER_EVENT_HANDLER(PluginProxy::MousePressHandler);
    QZ_REGISTER_EVENT_HANDLER(PluginProxy::MouseReleaseHandler);
}

void AutoScrollPlugin::unload()
{
    m_scroller->deleteLater();
}

bool AutoScrollPlugin::testPlugin()
{
    return (QupZilla::VERSION == QLatin1String("1.7.0"));
}

QTranslator* AutoScrollPlugin::getTranslator(const QString &locale)
{
    QTranslator* translator = new QTranslator(this);
    translator->load(locale, ":/autoscroll/locale/");
    return translator;
}

bool AutoScrollPlugin::mouseMove(const Qz::ObjectName &type, QObject* obj, QMouseEvent* event)
{
    if (type == Qz::ON_WebView) {
        return m_scroller->mouseMove(obj, event);
    }

    return false;
}

bool AutoScrollPlugin::mousePress(const Qz::ObjectName &type, QObject* obj, QMouseEvent* event)
{
    if (type == Qz::ON_WebView) {
        return m_scroller->mousePress(obj, event);
    }

    return false;
}

bool AutoScrollPlugin::mouseRelease(const Qz::ObjectName &type, QObject* obj, QMouseEvent* event)
{
    if (type == Qz::ON_WebView) {
        return m_scroller->mouseRelease(obj, event);
    }

    return false;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(AutoScroll, AutoScrollPlugin)
#endif