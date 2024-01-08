// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#include <QCryptographicHash>
#include <QDebug>
#include <QJsonObject>
#include <QJSValue>
#include <QJsonArray>

#include "data/selectedheaderstemplate.h"

namespace Arrival::App
{
    SelectedHeadersTemplate::SelectedHeadersTemplate()
        : m_headerIdentifier()
        , m_templateName()
        , m_headerIndices()
    {}

    SelectedHeadersTemplate::SelectedHeadersTemplate(const QString& headerIdentifier, const QString& templateName, const QList<int>& indices)
        : m_headerIdentifier(headerIdentifier)
        , m_templateName(templateName)
        , m_headerIndices(indices)
    {}

    SelectedHeadersTemplate::SelectedHeadersTemplate(const QString& templateName, const QList<QString>& headerNames, const QList<int>& indices)
    {
        // Get the hash.
        if (const auto result = generateUniqueIdentifierFromHeaderList(headerNames); result.has_value())
        {
            m_headerIdentifier = *result;
        }
        else if (result.error() == SelectedHeadersTemplate::UniqueIdentifierError::ListEmpty)
        {
            qDebug() << "Empty list passed to generateUniqueIdentifierFromHeaderList()";
            m_headerIdentifier = QString();
        }
        else if (result.error() == SelectedHeadersTemplate::UniqueIdentifierError::StringsEmpty)
        {
            qDebug() << "Empty strings passed to generateUniqueIdentifierFromHeaderList()";
            m_headerIdentifier = QString();
        }

        m_templateName = templateName;
        m_headerIndices = indices;
    }

    std::expected<SelectedHeadersTemplate, SelectedHeadersTemplate::FromJsonError> SelectedHeadersTemplate::fromJson(const QJsonObject& jsonObject)
    {
        // The result.
        SelectedHeadersTemplate result;

        // Get the header identifier.
        if (const QJsonValue value = jsonObject[headerIdentifierJsonName]; value.isString())
        {
            result.m_headerIdentifier = value.toString();
        }
        else
        {
            return std::unexpected<SelectedHeadersTemplate::FromJsonError>(SelectedHeadersTemplate::FromJsonError::ParseError);
        }

        // Get the template name.
        if (const QJsonValue value = jsonObject[templateNameJsonName]; value.isString())
        {
            result.m_templateName = value.toString();
        }
        else
        {
            return std::unexpected<SelectedHeadersTemplate::FromJsonError>(SelectedHeadersTemplate::FromJsonError::ParseError);
        }

        // Get the indices.
        if (const QJsonValue value = jsonObject[indicesJsonName]; value.isArray())
        {
            const QJsonArray indices = value.toArray();
            result.m_headerIndices.reserve(indices.count());
            for (const QJsonValue& index : indices)
            {
                if (!index.isDouble() && !qFuzzyCompare(index.toDouble(), static_cast<double>(index.toInt())))
                {
                    return std::unexpected<SelectedHeadersTemplate::FromJsonError>(SelectedHeadersTemplate::FromJsonError::ParseError);
                }
                result.m_headerIndices.append(index.toInt());
            }
        }
        else
        {
            return std::unexpected<SelectedHeadersTemplate::FromJsonError>(SelectedHeadersTemplate::FromJsonError::ParseError);
        }

        return result;
    }

    std::expected<QString, SelectedHeadersTemplate::UniqueIdentifierError> SelectedHeadersTemplate::generateUniqueIdentifierFromHeaderList(const QList<QString>& headerNames)
    {
        if (headerNames.empty())
        {
            return std::unexpected<SelectedHeadersTemplate::UniqueIdentifierError>(SelectedHeadersTemplate::UniqueIdentifierError::ListEmpty);
        }

        QString concatenatedString = headerNames.join("");
        if (concatenatedString.isEmpty())
        {
            return std::unexpected<SelectedHeadersTemplate::UniqueIdentifierError>(SelectedHeadersTemplate::UniqueIdentifierError::StringsEmpty);
        }

        QCryptographicHash hash(QCryptographicHash::Sha256);
        hash.addData(concatenatedString.toUtf8());
        QByteArray hashData = hash.result();
        QString uniqueHash = QString(hashData.toHex());

        return uniqueHash;
    }

    QJsonObject SelectedHeadersTemplate::toJson() const
    {
        QJsonObject result;
        result[headerIdentifierJsonName] = m_headerIdentifier;
        result[templateNameJsonName] = m_templateName;

        QJsonArray indicesArray;
        for (int index : m_headerIndices)
        {
            indicesArray.append(index);
        }
        result[indicesJsonName] = indicesArray;

        return result;
    }
}
