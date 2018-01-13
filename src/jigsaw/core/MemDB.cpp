#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QStandardPaths>
#include <QCryptographicHash>
#include "jigsaw/core/MemDB.h"

QHash<QString, jigsaw::core::MemDB*> jigsaw::core::MemDB::buckets;

//-----------------------------------------------------------------------
void jigsaw::core::MemDB::CreateBucket(const QString& _bucket)
{
	if (buckets.contains(_bucket))
		return;
	buckets.insert(_bucket, new jigsaw::core::MemDB(_bucket));
}

//-----------------------------------------------------------------------
void jigsaw::core::MemDB::DestroyBucket(const QString& _bucket)
{
	if (!buckets.contains(_bucket))
		return;
	delete buckets.value(_bucket);
	buckets.remove(_bucket);
}

//-----------------------------------------------------------------------
jigsaw::core::MemDB* jigsaw::core::MemDB::FindBucket(const QString& _bucket)
{
	if (buckets.contains(_bucket))
		return buckets.value(_bucket);
	return NULL;
}

//-----------------------------------------------------------------------
QByteArray jigsaw::core::MemDB::EncodeBucket(const QString& _bucket)
{
	jigsaw::core::MemDB* memDB = FindBucket(_bucket);
	if (NULL == memDB)
		return QByteArray();
	QJsonObject objKV;
	for (auto itr = memDB->kv.cbegin(); itr != memDB->kv.cend(); ++itr)
	{
		objKV.insert(itr.key(), QJsonValue::fromVariant(itr.value()));
	}

	QJsonObject objSets;
	for (auto itr = memDB->sets.cbegin(); itr != memDB->sets.cend(); ++itr)
	{
		QJsonArray arySets;
        foreach (QVariant value, itr.value())
		{
			arySets.append(QJsonValue::fromVariant(value));
		}
		objSets.insert(itr.key(), arySets);
	}

	QJsonObject objHash;
	for (auto itr = memDB->hash.cbegin(); itr != memDB->hash.cend(); ++itr)
	{
		QJsonObject objSubHash;
		for (auto itrSub = itr.value().cbegin(); itrSub != itr.value().cend(); ++itrSub)
		{
			objSubHash.insert(itrSub.key(), QJsonValue::fromVariant(itrSub.value()));
		}
		objHash.insert(itr.key(), objSubHash);
	}

	QJsonObject root;
	root.insert("bucket", _bucket);
	root.insert("kv", objKV);
	root.insert("sets", objSets);
	root.insert("hash", objHash);

	QJsonDocument doc;
	doc.setObject(root);
	return doc.toJson();
}

//-----------------------------------------------------------------------
void jigsaw::core::MemDB::DecodeBucket(const QString& _data)
{
	QJsonDocument doc = QJsonDocument::fromJson(_data.toUtf8());
	QJsonObject root = doc.object();
	QString bucket = root.take("bucket").toString();
	jigsaw::core::MemDB* memDB = FindBucket(bucket);
	//override a exists bucket is not allowed
	if (NULL != memDB)
		return;

	CreateBucket(bucket);
	memDB = FindBucket(bucket);

	QJsonObject objKV = root.take("kv").toObject();
    foreach (QString key, objKV.keys())
	{
		memDB->KVSet(key, objKV[key].toVariant());
	}

	QJsonObject objHash = root.take("hash").toObject();
    foreach (QString key, objHash.keys())
	{
		QJsonObject objField = objHash.take(key).toObject();
        foreach (QString field, objField.keys())
		{
			memDB->HashSet(key, field, objField[field].toVariant());
		}
	}

	QJsonObject objSets = root.take("sets").toObject();
    foreach (QString key, objSets.keys())
	{
		QJsonArray aryMember = objSets.take(key).toArray();
        foreach (QJsonValue member, aryMember)
		{
			memDB->SetsAdd(key, member.toVariant());
		}
	}

}

//-----------------------------------------------------------------------
jigsaw::core::MemDB::MemDB(const QString& _bucket)
{
	bucket = _bucket;
}

//-----------------------------------------------------------------------
QString jigsaw::core::MemDB::GetBucket()
{
	return bucket;
}

void jigsaw::core::MemDB::RemoveKV(const QString& _key)
{
	if (kv.contains(_key))
		kv.remove(_key);
}

//-----------------------------------------------------------------------
void jigsaw::core::MemDB::KVSet(const QString& _key, const QVariant& _value)
{
	kv[_key] = _value;
}

//-----------------------------------------------------------------------
QVariant jigsaw::core::MemDB::KVGet(const QString& _key)
{
	if (kv.contains(_key))
		return kv.value(_key);
	return QVariant();
}

//-----------------------------------------------------------------------
QList<QString> jigsaw::core::MemDB::KVKeys()
{
	return kv.keys();
}

void jigsaw::core::MemDB::RemoveSets(const QString& _key)
{
	if (sets.contains(_key))
		sets.remove(_key);
}

//-----------------------------------------------------------------------
void jigsaw::core::MemDB::SetsAdd(const QString& _key, const QVariant& _member)
{
	if (sets[_key].contains(_member))
		return;
	sets[_key].append(_member);
}

void jigsaw::core::MemDB::SetsRemove(const QString& _key, const QVariant& _member)
{
	if (!sets.contains(_key))
		return;

	if (!sets.value(_key).contains(_member))
		return;

	sets[_key].removeAll(_member);
}

//-----------------------------------------------------------------------
QList<QVariant> jigsaw::core::MemDB::SetsMembers(const QString _key)
{
	return sets.value(_key);
}

void jigsaw::core::MemDB::RemoveHash(const QString& _key)
{
	if (hash.contains(_key))
		hash.remove(_key);
}

//-----------------------------------------------------------------------
void jigsaw::core::MemDB::HashSet(const QString& _key, const QString& _field, const QVariant& _value)
{
	hash[_key][_field] = _value;
}
//-----------------------------------------------------------------------
QVariant jigsaw::core::MemDB::HashGet(const QString& _key, const QString& _field)
{
	if (!hash.contains(_key))
		return QVariant();
	if (!hash[_key].contains(_field))
		return QVariant();
	return hash.value(_key).value(_field);
}

//-----------------------------------------------------------------------
QVariant jigsaw::core::MemDB::HashGet(const QString& _key, const QString& _field, QVariant _default)
{
	if (!hash.contains(_key))
		return _default;
	if (!hash[_key].contains(_field))
		return _default;
	return hash.value(_key).value(_field);
}

//-----------------------------------------------------------------------
QList<QString> jigsaw::core::MemDB::HashKeys()
{
	return hash.keys();
}

//-----------------------------------------------------------------------
QList<QString> jigsaw::core::MemDB::HashFields(const QString& _key)
{
	QList<QString> fields;
	if (hash.contains(_key))
	{
		fields = hash.value(_key).keys();
	}
	return fields;
}

QString getBucketDumpFilePath(const QString& _bucket)
{
	QDir dir(jigsaw::core::MemDBUtil::GetDataPath());
	if (!dir.exists())
		dir.mkpath(jigsaw::core::MemDBUtil::GetDataPath());

	QString bucket = _bucket;
	return dir.absoluteFilePath(bucket.replace(":", ".").append(".vcb"));
}

//-----------------------------------------------------------------------
int jigsaw::core::MemDBUtil::SaveBucket(const QString& _bucket)
{
	jigsaw::core::MemDB* memDB = jigsaw::core::MemDB::FindBucket(_bucket);
	if (NULL == memDB)
		return 1;

	const QByteArray content = jigsaw::core::MemDB::EncodeBucket(_bucket);
	QString filepath = getBucketDumpFilePath(_bucket);
	QFile file(filepath);
	if (file.open(QIODevice::WriteOnly))
	{
		QDataStream  out(&file);
		out << content;
		file.close();
	}
	else
	{
		file.close();
		return 2;
	}
	return 0;
}

//-----------------------------------------------------------------------
int jigsaw::core::MemDBUtil::DeleteBucket(const QString& _bucket)
{
	QString filepath = getBucketDumpFilePath(_bucket);
	QFile::remove(filepath);
	return 0;
}

//-----------------------------------------------------------------------
int jigsaw::core::MemDBUtil::LoadBucket(const QString& _bucket)
{
	QString filepath = getBucketDumpFilePath(_bucket);
	QFile file(filepath);
	QString content;
	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray bytes;
		QDataStream  in(&file);
		in >> bytes;
		file.close();
		content = QString::fromUtf8(bytes);
	}
	else
	{
		file.close();
		return 2;
	}

	jigsaw::core::MemDB::DecodeBucket(content);
	return 0;
}

//-----------------------------------------------------------------------
QString jigsaw::core::MemDBUtil::GetDataPath()
{
	QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	if (!dir.exists("memdb_dump"))
		dir.mkdir("memdb_dump");
	dir.cd("memdb_dump");
	return dir.absolutePath();
}

//-----------------------------------------------------------------------
QByteArray jigsaw::core::MemDBUtil::TakeUUID(const QString& _bucket)
{
	QString bucket = _bucket;
	bucket = bucket.remove(0, bucket.indexOf("[") + 1);
	bucket = bucket.remove(bucket.indexOf("]"), bucket.size() - bucket.indexOf("]"));
	return bucket.toUtf8();
}
