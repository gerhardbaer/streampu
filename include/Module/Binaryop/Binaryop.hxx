#include "Module/Binaryop/Binaryop.hpp"

namespace aff3ct
{
namespace module
{

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
runtime::Task& Binaryop<TI,TO,BOP>
::operator[](const bop::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
runtime::Socket& Binaryop<TI,TO,BOP>
::operator[](const bop::sck::perform s)
{
	return Module::operator[]((size_t)bop::tsk::perform)[(size_t)s];
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
runtime::Socket& Binaryop<TI,TO,BOP>
::operator[](const bop::sck::performf s)
{
	return Module::operator[]((size_t)bop::tsk::performf)[(size_t)s];
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
runtime::Socket& Binaryop<TI,TO,BOP>
::operator[](const std::string &tsk_sck)
{
	return Module::operator[](tsk_sck);
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
Binaryop<TI,TO,BOP>
::Binaryop(const size_t n_in0, const size_t n_in1)
: Module(), n_elmts(n_in0>n_in1?n_in0:n_in1)
{
	const std::string name = "Binaryop";
	this->set_name(name + "<" + tools::bop_get_name<TI,TO,BOP>() + ">");
	this->set_short_name(name);

	if (n_in0 == 0)
	{
		std::stringstream message;
		message << "'n_in0' has to be greater than 0 ('n_in0' = " << n_in0 << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_in1 == 0)
	{
		std::stringstream message;
		message << "'n_in1' has to be greater than 0 ('n_in1' = " << n_in1 << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_in0 != n_in1 && n_in0 != 1 && n_in1 != 1)
	{
		std::stringstream message;
		message << "'n_in0' and 'n_in1' have to be equal, or one the two should be 1 ('n_in0' = " << n_in0 << ",'n_in1' = " << n_in1 << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("perform");
	auto ps_in0 = this->template create_socket_in <TI>(p, "in0", n_in0);
	auto ps_in1 = this->template create_socket_in <TI>(p, "in1", n_in1);
	auto ps_out = this->template create_socket_out<TO>(p, "out", this->n_elmts);

	if (n_in0 == 1)
	{
	this->create_codelet(p, [ps_in0, ps_in1, ps_out](Module &m, runtime::Task &t, const size_t frame_id) -> int
	{
		auto &bop = static_cast<Binaryop&>(m);
		bop._perform(*static_cast<const TI*>(t[ps_in0].get_dataptr()),
		              static_cast<const TI*>(t[ps_in1].get_dataptr()),
		              static_cast<      TO*>(t[ps_out].get_dataptr()),
		             frame_id);
		return runtime::status_t::SUCCESS;
	});
	}
	else if (n_in1 == 1)
	{
	this->create_codelet(p, [ps_in0, ps_in1, ps_out](Module &m, runtime::Task &t, const size_t frame_id) -> int
	{
		auto &bop = static_cast<Binaryop&>(m);
		bop._perform( static_cast<const TI*>(t[ps_in0].get_dataptr()),
		             *static_cast<const TI*>(t[ps_in1].get_dataptr()),
		              static_cast<      TO*>(t[ps_out].get_dataptr()),
		              frame_id);
		return runtime::status_t::SUCCESS;
	});
	}
	else
	{
	this->create_codelet(p, [ps_in0, ps_in1, ps_out](Module &m, runtime::Task &t, const size_t frame_id) -> int
	{
		auto &bop = static_cast<Binaryop&>(m);
		bop._perform(static_cast<const TI*>(t[ps_in0].get_dataptr()),
		             static_cast<const TI*>(t[ps_in1].get_dataptr()),
		             static_cast<      TO*>(t[ps_out].get_dataptr()),
		             frame_id);
		return runtime::status_t::SUCCESS;
	});
	}

	auto &p2 = this->create_task("performf");
	auto p2_in0 = this->template create_socket_fwd <TI>(p2, "in0", n_in0  );
	auto p2_in1 = this->template create_socket_in  <TI>(p2, "in1", n_in1  );

	if (n_in1 == 1)
	{
	this->create_codelet(p2, [p2_in0, p2_in1](Module &m, runtime::Task &t, const size_t frame_id) -> int
	{
		auto &bop = static_cast<Binaryop&>(m);
		bop._perform( static_cast<      TI*>(t[p2_in0].get_dataptr()),
		             *static_cast<const TI*>(t[p2_in1].get_dataptr()),
		             frame_id);
		return runtime::status_t::SUCCESS;
	});
	}
	else
	{
	this->create_codelet(p2, [p2_in0, p2_in1](Module &m, runtime::Task &t, const size_t frame_id) -> int
	{
		auto &bop = static_cast<Binaryop&>(m);
		bop._perform(static_cast<      TI*>(t[p2_in0].get_dataptr()),
		             static_cast<const TI*>(t[p2_in1].get_dataptr()),
		             frame_id);
		return runtime::status_t::SUCCESS;
	});
	}
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
Binaryop<TI,TO,BOP>
::Binaryop(const size_t n_elmts)
: Binaryop<TI,TO,BOP>(n_elmts,n_elmts)
{
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
Binaryop<TI,TO,BOP>* Binaryop<TI,TO,BOP>
::clone() const
{
	auto m = new Binaryop(*this);
	m->deep_copy(*this);
	return m;
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
size_t Binaryop<TI,TO,BOP>
::get_n_elmts() const
{
	return this->n_elmts;
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
template <class AI, class AO>
void Binaryop<TI,TO,BOP>
::perform(const std::vector<TI,AI>& in0,
          const std::vector<TI,AI>& in1,
                std::vector<TO,AO>& out,
          const int frame_id,
          const bool managed_memory)
{
	(*this)[bop::sck::perform::in0].bind(in0);
	(*this)[bop::sck::perform::in0].bind(in1);
	(*this)[bop::sck::perform::out].bind(out);
	(*this)[bop::tsk::perform].exec(frame_id, managed_memory);
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
void Binaryop<TI,TO,BOP>
::perform(const TI *in0, const TI *in1, TO *out, const int frame_id, const bool managed_memory)
{
	(*this)[bop::sck::perform::in0].bind(in0);
	(*this)[bop::sck::perform::in0].bind(in1);
	(*this)[bop::sck::perform::out].bind(out);
	(*this)[bop::tsk::perform].exec(frame_id, managed_memory);
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
void Binaryop<TI,TO,BOP>
::_perform(const TI *in0, const TI *in1, TO *out, const size_t frame_id)
{
	for (size_t e = 0; e < this->n_elmts; e++)
		out[e] = BOP(in0[e], in1[e]);
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
void Binaryop<TI,TO,BOP>
::_perform(TI *in0, const TI *in1, const size_t frame_id)
{
	for (size_t e = 0; e < this->n_elmts; e++)
		in0[e] = BOP(in0[e], in1[e]);
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
void Binaryop<TI,TO,BOP>
::_perform(const TI in0, const TI *in1, TO *out, const size_t frame_id)
{
	for (size_t e = 0; e < this->n_elmts; e++)
		out[e] = BOP(in0, in1[e]);
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
void Binaryop<TI,TO,BOP>
::_perform(const TI *in0, const TI in1, TO *out, const size_t frame_id)
{
	for (size_t e = 0; e < this->n_elmts; e++)
		out[e] = BOP(in0[e], in1);
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
void Binaryop<TI,TO,BOP>
::_perform(TI *in0, const TI in1, const size_t frame_id)
{
	for (size_t e = 0; e < this->n_elmts; e++)
		in0[e] = BOP(in0[e], in1);
}
}
}
