// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#ifndef ARRIVAL_SELECTEDHEADERSTEMPLATE_H
#define ARRIVAL_SELECTEDHEADERSTEMPLATE_H

#include <QString>
#include <QList>
#include <QJsonObject>

#include <expected>

namespace Arrival::App
{
    class SelectedHeadersTemplate
    {
    public:
        enum class FromJsonError
        {
            ParseError
        };

        enum class UniqueIdentifierError
        {
            ListEmpty,
            StringsEmpty
        };

        static constexpr const char* headerIdentifierJsonName = "headerId";
        static constexpr const char* templateNameJsonName = "templateName";
        static constexpr const char* indicesJsonName = "indices";

    public:
        /*!
         * \brief SelectedHeadersTemplate default constructor.
         */
        SelectedHeadersTemplate();

        /*!
         * \brief SelectedHeadersTemplate constructs a new \c SelectedHeadersTemplate.
         * \param headerIdentifier The header identifier.
         * \param templateName The template name.
         * \param indices The indices.
         */
        SelectedHeadersTemplate(const QString& headerIdentifier, const QString& templateName, const QList<int>& indices);

        /*!
         * \brief SelectedHeadersTemplate constructs a new \c SelectedHeadersTemplate.
         * \param templateName The template name.
         * \param headerNames List of all headers to construct this template for.
         * \param indices The indices.
         */
        SelectedHeadersTemplate(const QString& templateName, const QList<QString>& headerNames, const QList<int>& indices);

        /*!
         * \brief fromJson creates a \c SelectedHeadersTemplate from a \c QJsonObject.
         * \param fromObject The \c QJsonObject to create the \c SelectedHeadersTemplate instance from.
         * \return \c std::expected holding the result or an error.
         */
        static std::expected<SelectedHeadersTemplate, FromJsonError> fromJson(const QJsonObject& fromObject);

        /*!
         * \brief generateUniqueIdentifierFromHeaderList generates a unique identifier for a list of header names.
         * \param headerNames List containg the headers.
         * \return Hash of the headers stored in a \c QString.
         */
        static std::expected<QString, UniqueIdentifierError> generateUniqueIdentifierFromHeaderList(const QList<QString>& headerNames);

        /*!
         * \brief toJson Converts the instance into a \c QJsonObject object.
         * \return The \c QJsonObject
         */
        QJsonObject toJson() const;

        const QString& headerIdentifier() const
        {
            return m_headerIdentifier;
        }

        const QString& templateName() const
        {
            return m_templateName;
        }

        const QList<int>& headerIndices() const
        {
            return m_headerIndices;
        }

    private:
        /*!
         * \brief m_headerIdentifier is created by a hash function that hashes all
         * the headers in a .csv document to generate a unique if for a .csv format.
         */
        QString m_headerIdentifier;

        /*!
         * \brief m_templateName is given by the user to a template so that it can be saved
         * and used later.
         */
        QString m_templateName;

        /*!
         * \brief m_headerIndices The indices stored in this template.
         */
        QList<int> m_headerIndices;
    };

    inline bool operator==(const SelectedHeadersTemplate& lhs, const SelectedHeadersTemplate& rhs)
    {
        return lhs.headerIdentifier() == rhs.headerIdentifier() &&  lhs.templateName() == rhs.templateName() && lhs.headerIndices() == rhs.headerIndices();
    }

    inline bool operator!=(const SelectedHeadersTemplate& lhs, const SelectedHeadersTemplate& rhs)
    {
        return !(lhs == rhs);
    }
}

#endif // ARRIVAL_SELECTEDHEADERSTEMPLATE_H
