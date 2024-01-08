// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <algorithm>

#include "data/selectedheaderstemplatelist.h"

namespace Arrival::App
{
    SelectedHeadersTemplateList::SelectedHeadersTemplateList(QObject* parent)
        : QObject(parent)
    {}

    QList<int> SelectedHeadersTemplateList::getTemplateIndices(int index) const
    {
        return m_templates.at(index).headerIndices();
    }

    void SelectedHeadersTemplateList::saveToJson(const QString& file) const
    {
        if (file.isEmpty())
        {
            qWarning() << "Empty file name";
            return;
        }

        QJsonArray jsonArray;
        for (const auto& item : m_templates)
        {
            jsonArray.append(item.toJson());
        }
        QJsonDocument jsonDocument(jsonArray);

        QFile jsonSaveFile(file);
        if (!jsonSaveFile.open(QIODevice::WriteOnly))
        {
            qWarning() << "Unable to open file: " + file;
            return;
        }
        jsonSaveFile.write(jsonDocument.toJson());
        jsonSaveFile.close();
    }

    void SelectedHeadersTemplateList::loadFromJson(const QString& file)
    {
        if (file.isEmpty())
        {
            qWarning() << "Empty file name";
            return;
        }

        // It is possible that the file does not exist.
        // No loading then.
        QFileInfo jsonFileInfo(file);
        if (!jsonFileInfo.exists())
        {
            return;
        }

        QFile jsonSaveFile(file);
        if (!jsonSaveFile.open(QIODevice::ReadOnly))
        {
            qWarning() << "Unable to open file: " + file;
            return;
        }
        QByteArray jsonData = jsonSaveFile.readAll();
        jsonSaveFile.close();

        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonData);
        if (jsonDocument.isNull())
        {
            qWarning() << "Error parsing json file: " + file;
            return;
        }

        // The model is resetted when loading a file.
        emit preListReset();

        // Clear any entries.
        m_templates.clear();

        QJsonArray jsonArray = jsonDocument.array();
        for (const QJsonValue& value : jsonArray)
        {
            if (!value.isObject())
            {
                qWarning() << "Error parsing json file: " + file;
                continue;
            }
            if (const auto result = SelectedHeadersTemplate::fromJson(value.toObject()); result.has_value())
            {
                m_templates.append(result.value());
            }
            else
            {
                qWarning() << "Error parsing json file: " + file;
                continue;
            }
        }

        // Finishes resetting the model.
        emit postListReset();
    }

    void SelectedHeadersTemplateList::addNewTemplate(const QString& formatIdentifier, const QString& templateName, const QList<int>& indices)
    {
        SelectedHeadersTemplate newTemplate(formatIdentifier, templateName, indices);
        appendItem(newTemplate);

        saveToJson(templateFileName);
    }

    void SelectedHeadersTemplateList::appendItem(const SelectedHeadersTemplate& itemTemplate)
    {
        emit preItemAppend();
        m_templates.append(itemTemplate);
        emit postItemAppend();
    }

    void SelectedHeadersTemplateList::removeItemAt(int index)
    {
        if (index < 0 || index >= m_templates.count())
        {
            return;
        }

        emit preItemRemove(index);
        m_templates.removeAt(index);
        emit postItemRemove();

        saveToJson(templateFileName);
    }

    void SelectedHeadersTemplateList::removeItem(const SelectedHeadersTemplate& templateItem)
    {
        if (!m_templates.contains(templateItem))
        {
            return;
        }

        const auto& iterator = std::find(m_templates.begin(), m_templates.end(), templateItem);
        const int index = std::distance(m_templates.begin(), iterator);

        emit preItemRemove(index);
        m_templates.removeOne(templateItem);
        emit postItemRemove();

        saveToJson(templateFileName);
    }

    void SelectedHeadersTemplateList::clearList()
    {
        emit preListReset();
        m_templates.clear();
        emit postListReset();
    }
}
