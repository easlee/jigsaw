#pragma once

#include <QVariant>
#include <QHash>
#include <QString>

namespace jigsaw
{
	namespace core
	{
		class MemDB
		{
		public:
			static void CreateBucket(const QString& _bucket);
			static void DestroyBucket(const QString& _bucket);
			static MemDB* FindBucket(const QString& _bucket);

			static QByteArray EncodeBucket(const QString& _bucket);
			static void DecodeBucket(const QString& _data);

			MemDB(const QString& _bucket);

			QString GetBucket();

			void RemoveKV(const QString& _key);
			void KVSet(const QString& _key, const QVariant& _value);
			QVariant KVGet(const QString& _key);
			QList<QString> KVKeys();

			void RemoveSets(const QString& _key);

			void SetsAdd(const QString& _key, const QVariant& _member);
			void SetsRemove(const QString& _key, const QVariant& _member);
			QList<QVariant> SetsMembers(const QString _key);

			void RemoveHash(const QString& _key);
			void HashSet(const QString& _key, const QString& _field, const QVariant& _value);
			QVariant HashGet(const QString& _key, const QString& _field);
			QVariant HashGet(const QString& _key, const QString& _field, QVariant _default);
			QList<QString> HashKeys();
			QList<QString> HashFields(const QString& _key);
		private:
			static QHash<QString, MemDB*> buckets;
			QString bucket;
			QHash<QString, QHash<QString, QVariant> > hash;
			QHash<QString, QList<QVariant> > sets;
			QHash<QString, QVariant > kv;
		};

		/// \brief
		/// default path of dump is QStandardPaths::AppDataLocation
		class MemDBUtil
		{
		public:

			/// \return error code
			/// 0 : OK
			/// 1 : bucket is not exists
			/// 2 : file open failed
			static int SaveBucket(const QString& _bucket);

			/// \return error code
			/// 0 : OK
			static int DeleteBucket(const QString& _bucket);

			/// \return error code
			/// 0 : OK
			/// 2 : file open failed
			static int LoadBucket(const QString& _bucket);

			static QString GetDataPath();
			static QByteArray TakeUUID(const QString& _bucket);
		protected:
		private:
		};
	} //namespace core
} //namespace jigsaw


#define TAG(x) \
namespace Tags \
{ \
static const QString& x = #x; \
}

#define BUCKET_FMT(_prefix,	_suffix) \
	const QString bucket_fmt = #_prefix":%1:"#_suffix

#define BUCKET(x) \
	const QString bucket = #x; \
	const QString bucket_fmt = "%1"

#define  DECLARE_INITDB \
	void InitializeModel(const QString& _uuid)

#define  IMPLEMENT_INITDB \
	void InitializeModel(const QString& _uuid) \
	{ \
		jigsaw::core::MemDB::CreateBucket(QString(bucket_fmt).arg(_uuid)); \
	}

#define DECLARE_GETDB \
	jigsaw::core::MemDB* GetMemDB(const QString& _uuid)

#define IMPLEMENT_GETDB \
	jigsaw::core::MemDB* GetMemDB(const QString& _uuid) \
	{ \
		return jigsaw::core::MemDB::FindBucket(QString(bucket_fmt).arg(_uuid)); \
	}


//////////////////////////////////////////////////////////////////////////
// KV
//////////////////////////////////////////////////////////////////////////
#define DECLARE_KV(var) \
	namespace KV { \
		namespace Query { \
			QVariant var(jigsaw::core::MemDB* _memDB); \
		}\
		namespace Update { \
			void var(jigsaw::core::MemDB* _memDB, const QVariant& _value); \
		} \
		namespace Remove { \
			void var(jigsaw::core::MemDB* _memDB); \
		} \
	}


#define IMPLEMENT_KV(var, flag) \
	namespace KVKeys { \
		static const QString& var = #var; \
	}\
	namespace KV { \
		namespace Query { \
			QVariant var(jigsaw::core::MemDB* _memDB) \
			{	\
				return _memDB->KVGet(KVKeys::var); \
			} \
		}\
		namespace Update { \
			void var(jigsaw::core::MemDB* _memDB, const QVariant& _value) \
			{ \
				_memDB->KVSet(KVKeys::var, _value); \
				if(flag) \
					jigsaw::core::MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
		namespace Remove { \
			void var(jigsaw::core::MemDB* _memDB) \
			{ \
				_memDB->RemoveKV(KVKeys::var); \
				if(flag) \
					jigsaw::core::MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}


#define DECLARE_KV_1(var) \
	namespace KV { \
		namespace Query { \
			QVariant var(jigsaw::core::MemDB* _memDB, const QString _1); \
		}\
		namespace Update { \
			void var(jigsaw::core::MemDB* _memDB, const QString& _1, const QVariant& _value); \
		} \
		namespace Remove { \
			void var(jigsaw::core::MemDB* _memDB, const QString& _1); \
		} \
	}


#define IMPLEMENT_KV_1(var, flag) \
	namespace KVKeys { \
		static const QString& var = #var":[%1]"; \
	}\
	namespace KV { \
		namespace Query { \
			QVariant var(jigsaw::core::MemDB* _memDB, const QString _1) \
			{	\
				return _memDB->KVGet(QString(KVKeys::var).arg(_1)); \
			} \
		}\
		namespace Update { \
			void var(jigsaw::core::MemDB* _memDB, const QString& _1, const QVariant& _value) \
			{ \
				_memDB->KVSet(QString(KVKeys::var).arg(_1), _value); \
				if(flag) \
					jigsaw::core::MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
		namespace Remove { \
			void var(jigsaw::core::MemDB* _memDB, const QString& _1) \
			{ \
				_memDB->RemoveKV(QString(KVKeys::var).arg(_1)); \
				if(flag) \
					jigsaw::core::MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}

#define DECLARE_KV_2(var) \
	namespace KV { \
		namespace Query { \
			QVariant var(jigsaw::core::MemDB* _memDB, const QString _1, const QString _2); \
		}\
		namespace Update { \
			void var(jigsaw::core::MemDB* _memDB, const QString& _1, const QString _2, const QVariant& _value); \
		} \
		namespace Remove { \
			void var(jigsaw::core::MemDB* _memDB, const QString& _1, const QString _2); \
		} \
	}

#define IMPLEMENT_KV_2(var, flag) \
	namespace KVKeys { \
		static const QString& var = #var":[%1]:[%2]"; \
	}\
	namespace KV { \
		namespace Query { \
			QVariant var(jigsaw::core::MemDB* _memDB, const QString _1, const QString _2) \
			{	\
				return _memDB->KVGet(QString(KVKeys::var).arg(_1).arg(_2)).toString(); \
			} \
		}\
		namespace Update { \
			void var(jigsaw::core::MemDB* _memDB, const QString& _1, const QString _2, const QVariant& _value) \
			{ \
				_memDB->KVSet(QString(KVKeys::var).arg(_1).arg(_2), _value); \
				if(flag) \
					jigsaw::core::MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
		namespace Remove { \
			void var(jigsaw::core::MemDB* _memDB, const QString& _1, const QString _2) \
			{ \
				_memDB->RemoveKV(QString(KVKeys::var).arg(_1).arg(_2)); \
				if(flag) \
					jigsaw::core::MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}

//////////////////////////////////////////////////////////////////////////
// Sets
//////////////////////////////////////////////////////////////////////////

#define DECLARE_SETS(var) \
	namespace Sets { \
		namespace Query { \
			QList<QVariant> var(jigsaw::core::MemDB* _memDB); \
		}\
		namespace Update { \
			namespace Add { \
				void var(jigsaw::core::MemDB* _memDB, const QVariant& _value); \
			} \
			namespace Remove { \
				void var(jigsaw::core::MemDB* _memDB, const QVariant& _value); \
			} \
		} \
		namespace Remove { \
			void var(jigsaw::core::MemDB* _memDB); \
		} \
	}


#define IMPLEMENT_SETS(var, flag) \
	namespace SetsKeys { \
		static const QString& var = #var; \
	}\
	namespace Sets { \
		namespace Query { \
			QList<QVariant> var(jigsaw::core::MemDB* _memDB) \
			{	\
				return _memDB->SetsMembers(SetsKeys::var); \
			} \
		}\
		namespace Update { \
			namespace Add { \
				void var(jigsaw::core::MemDB* _memDB, const QVariant& _value) \
				{ \
					_memDB->SetsAdd(SetsKeys::var, _value); \
					if(flag) \
						jigsaw::core::MemDBUtil::SaveBucket(_memDB->GetBucket()); \
				} \
			} \
			namespace Remove { \
				void var(jigsaw::core::MemDB* _memDB, const QVariant& _value) \
				{ \
					_memDB->SetsRemove(SetsKeys::var, _value); \
					if(flag) \
						jigsaw::core::MemDBUtil::SaveBucket(_memDB->GetBucket()); \
				} \
			} \
		} \
		namespace Remove { \
			void var(jigsaw::core::MemDB* _memDB) \
			{ \
				_memDB->RemoveSets(SetsKeys::var); \
				if(flag) \
					jigsaw::core::MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}


#define DECLARE_SETS_1(var) \
	namespace Sets { \
		namespace Query { \
			QList<QVariant> var(jigsaw::core::MemDB* _memDB, const QString _1); \
		}\
		namespace Update { \
			namespace Add { \
				void var(jigsaw::core::MemDB* _memDB, const QString& _1, const QVariant& _value); \
			} \
			namespace Remove { \
				void var(jigsaw::core::MemDB* _memDB, const QString& _1, const QVariant& _value); \
			} \
		} \
		namespace Remove { \
			void var(jigsaw::core::MemDB* _memDB, const QString& _1); \
		} \
	}



#define IMPLEMENT_SETS_1(var, flag) \
	namespace SetsKeys { \
		static const QString& var = #var":[%1]"; \
	}\
	namespace Sets { \
		namespace Query { \
			QList<QVariant> var(jigsaw::core::MemDB* _memDB, const QString _1) \
			{	\
				return _memDB->SetsMembers(QString(SetsKeys::var).arg(_1)); \
			} \
		}\
		namespace Update { \
			namespace Add { \
				void var(jigsaw::core::MemDB* _memDB, const QString& _1, const QVariant& _value) \
				{ \
					_memDB->SetsAdd(QString(SetsKeys::var).arg(_1), _value); \
					if(flag) \
						jigsaw::core::MemDBUtil::SaveBucket(_memDB->GetBucket()); \
				} \
			} \
			namespace Remove { \
				void var(jigsaw::core::MemDB* _memDB, const QString& _1, const QVariant& _value) \
				{ \
					_memDB->SetsRemove(QString(SetsKeys::var).arg(_1), _value); \
					if(flag) \
						jigsaw::core::MemDBUtil::SaveBucket(_memDB->GetBucket()); \
				} \
			} \
		} \
		namespace Remove { \
			void var(jigsaw::core::MemDB* _memDB, const QString& _1) \
			{ \
				_memDB->RemoveSets(QString(SetsKeys::var).arg(_1)); \
				if(flag) \
					jigsaw::core::MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}

#define DECLARE_HASH(var) \
	namespace Hash { \
		namespace List { \
			QList<QString> var(jigsaw::core::MemDB* _memDB); \
		} \
		namespace Query { \
			QVariant var(jigsaw::core::MemDB* _memDB, const QString& _field); \
		}\
		namespace Update { \
			void var(jigsaw::core::MemDB* _memDB, const QString& _field, const QVariant& _value); \
		} \
		namespace Remove { \
			void var(jigsaw::core::MemDB* _memDB); \
		} \
	}


#define IMPLEMENT_HASH(var, flag) \
	namespace HashKeys { \
		static const QString& var = #var; \
	}\
	namespace Hash { \
		namespace List { \
			QList<QString> var(jigsaw::core::MemDB* _memDB) \
			{ \
				return _memDB->HashFields(HashKeys::var); \
			} \
		} \
		namespace Query { \
			QVariant var(jigsaw::core::MemDB* _memDB, const QString& _field) { \
				return _memDB->HashGet(HashKeys::var, _field); \
			} \
		}\
		namespace Update { \
			void var(jigsaw::core::MemDB* _memDB, const QString& _field, const QVariant& _value){ \
				_memDB->HashSet(HashKeys::var, _field, _value); \
				if(flag) \
					jigsaw::core::MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
		namespace Remove { \
			void var(jigsaw::core::MemDB* _memDB){ \
				_memDB->RemoveHash(HashKeys::var); \
				if(flag) \
					jigsaw::core::MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}


#define DECLARE_HASH_1(var) \
	namespace Hash { \
		namespace List { \
			QList<QString> var(jigsaw::core::MemDB* _memDB, const QString& _1); \
		} \
		namespace Query { \
			QVariant var(jigsaw::core::MemDB* _memDB, const QString& _1, const QString& _field); \
		}\
		namespace Update { \
			void var(jigsaw::core::MemDB* _memDB, const QString& _1, const QString& _field, const QVariant& _value); \
		} \
		namespace Remove { \
			void var(jigsaw::core::MemDB* _memDB, const QString& _1); \
		} \
	}


#define IMPLEMENT_HASH_1(var, flag) \
	namespace HashKeys { \
		static const QString& var = #var":[%1]"; \
	}\
	namespace Hash { \
		namespace List { \
			QList<QString> var(jigsaw::core::MemDB* _memDB, const QString& _1) \
			{ \
				return _memDB->HashFields(QString(HashKeys::var).arg(_1)); \
			} \
		} \
		namespace Query { \
			QVariant var(jigsaw::core::MemDB* _memDB, const QString& _1, const QString& _field) { \
				return _memDB->HashGet(QString(HashKeys::var).arg(_1), _field); \
			} \
		}\
		namespace Update { \
			void var(jigsaw::core::MemDB* _memDB, const QString& _1, const QString& _field, const QVariant& _value){ \
				_memDB->HashSet(QString(HashKeys::var).arg(_1), _field, _value); \
				if(flag) \
					jigsaw::core::MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
		namespace Remove { \
			void var(jigsaw::core::MemDB* _memDB, const QString& _1){ \
				_memDB->RemoveHash(QString(HashKeys::var).arg(_1)); \
				if(flag) \
					jigsaw::core::MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}
