#pragma once

#include <QString>
#include <QHash>

namespace jigsaw
{
	namespace core
	{
		class Translator
		{
		public:
			static void LoadSystemLanguage();
			static void LoadLanguage(const QString& _language);
			static QString Translate(const QString& _text);
		private:
			static QHash<QString, QString> kvmap;
		}; // class Translator
	} // namespace core
} // namespace jigsaw


#define TR(x) jigsaw::core::Translator::Translate(x)
