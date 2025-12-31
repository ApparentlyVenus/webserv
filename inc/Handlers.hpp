#ifndef HANDLERS_HPP
# define HANDLERS_HPP

# include "Response.hpp"
# include "Request.hpp"
# include "LocationConfig.hpp"
# include "FileUtils.hpp"
# include "StringUtils.hpp"
# include <fstream>
# include <sstream>
# include "../inc/ResponseStatus.hpp"

class Handlers
{
	public:
		static Response handleGet(Request &req, Response &res, const LocationConfig &conf);
		static Response handlePost(Request &req, Response &res, const LocationConfig &conf);
		static Response handleDelete(Request &req, Response &res, const LocationConfig &conf);

		static Response router(Response &res, Request &req, const LocationConfig &conf);

	private:
		Handlers();
		Handlers(const Handlers &other);
		Handlers &operator=(const Handlers &other);
		~Handlers();
};

#endif