\encoding{UTF-8}
\name{UUIDvalidate}
\alias{UUIDvalidate}
\title{
  Validate UUIDs
}
\description{
  \code{UUIDvalidate} validates Universally Unique Identifier strings.
}
\usage{
UUIDvalidate(uuid)
}
\arguments{
  \item{uuid}{character vector of UUID strings.}
}
\details{
  Rather than using regular expressions, this function uses \code{libuuid}'s
  \code{uuid_parse} function for efficiency.

  NA strings in `uuid` lead to NAs in the output vector.
}
\value{
  Logical vector, possibly containing NA values (see details).
}
%\references{
%}
\author{
  Murat Taşan, using libuuid by Theodore Ts'o.
}
%\note{
%}
\examples{
uuids <- sapply(1:10, UUIDgenerate)
uuids[3] <- NA
uuids[6] <- "foo"
UUIDvalidate(uuids)
UUIDvalidate(character(0))
}
