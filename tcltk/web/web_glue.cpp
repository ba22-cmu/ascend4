
#include <emscripten.h>
#include <stdlib.h>

EM_JS_DEPS(webidl_binder, "$intArrayFromString,$UTF8ToString,$alignMemory,$addOnInit");

extern "C" {

// Define custom allocator functions that we can force export using
// EMSCRIPTEN_KEEPALIVE.  This avoids all webidl users having to add
// malloc/free to -sEXPORTED_FUNCTIONS.
EMSCRIPTEN_KEEPALIVE void webidl_free(void* p) { free(p); }
EMSCRIPTEN_KEEPALIVE void* webidl_malloc(size_t len) { return malloc(len); }


// Interface: VoidPtr


void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

// Interface: rt


int EMSCRIPTEN_KEEPALIVE emscripten_bind_rt_get_rc_0(rt* self) {
  return self->rc;
}

const char* EMSCRIPTEN_KEEPALIVE emscripten_bind_rt_get_v_0(rt* self) {
  return self->v;
}

vtype EMSCRIPTEN_KEEPALIVE emscripten_bind_rt_get_t_0(rt* self) {
  return self->t;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_rt___destroy___0(rt* self) {
  delete self;
}

// Interface: ascjson


ascjson* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_ascjson_1(char* vargv) {
  return new ascjson(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_libr_query_1(ascjson* self, char* vargv) {
  return self->libr_query(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_asc_compiler_option_1(ascjson* self, char* vargv) {
  return self->asc_compiler_option(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_libr_parsestring_1(ascjson* self, char* vargv) {
  return self->libr_parsestring(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_librread_1(ascjson* self, char* vargv) {
  return self->librread(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_libr_moduleinfo_1(ascjson* self, char* vargv) {
  return self->libr_moduleinfo(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_libr_destroy_types_1(ascjson* self, char* vargv) {
  return self->libr_destroy_types(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_libr_hide_type_1(ascjson* self, char* vargv) {
  return self->libr_hide_type(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_libr_unhide_type_1(ascjson* self, char* vargv) {
  return self->libr_unhide_type(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_libr_type_is_shown_1(ascjson* self, char* vargv) {
  return self->libr_type_is_shown(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_libr_types_in_module_1(ascjson* self, char* vargv) {
  return self->libr_types_in_module(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_Asc_ExtractTypeHC_1(ascjson* self, char* vargv) {
  return self->Asc_ExtractTypeHC(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_qlfdid_1(ascjson* self, char* vargv) {
  return self->qlfdid(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_bgetproc_1(ascjson* self, char* vargv) {
  return self->bgetproc(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_brow_runmethod_1(ascjson* self, char* vargv) {
  return self->brow_runmethod(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_brow_assign_1(ascjson* self, char* vargv) {
  return self->brow_assign(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_qassgn3_1(ascjson* self, char* vargv) {
  return self->qassgn3(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__brow_iname_1(ascjson* self, char* vargv) {
  return self->x__brow_iname(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__brow_isrelation_1(ascjson* self, char* vargv) {
  return self->x__brow_isrelation(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__brow_ismodel_1(ascjson* self, char* vargv) {
  return self->x__brow_ismodel(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_bgetrels_1(ascjson* self, char* vargv) {
  return self->bgetrels(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_bgetrelspf_1(ascjson* self, char* vargv) {
  return self->bgetrelspf(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__brow_relsforatom_1(ascjson* self, char* vargv) {
  return self->x__brow_relsforatom(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__brow_islogrel_1(ascjson* self, char* vargv) {
  return self->x__brow_islogrel(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_bgetlogrels_1(ascjson* self, char* vargv) {
  return self->bgetlogrels(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_bgetlogrelspf_1(ascjson* self, char* vargv) {
  return self->bgetlogrelspf(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__brow_logrelsforatom_1(ascjson* self, char* vargv) {
  return self->x__brow_logrelsforatom(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_bgetcondrels_1(ascjson* self, char* vargv) {
  return self->bgetcondrels(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_bgetcondlogrels_1(ascjson* self, char* vargv) {
  return self->bgetcondlogrels(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__brow_iswhen_1(ascjson* self, char* vargv) {
  return self->x__brow_iswhen(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__brow_isinstanceinwhen_1(ascjson* self, char* vargv) {
  return self->x__brow_isinstanceinwhen(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_bgetwhens_1(ascjson* self, char* vargv) {
  return self->bgetwhens(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__brow_whensforinstance_1(ascjson* self, char* vargv) {
  return self->x__brow_whensforinstance(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_bwritependings_1(ascjson* self, char* vargv) {
  return self->bwritependings(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_bnumpendings_1(ascjson* self, char* vargv) {
  return self->bnumpendings(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_count_names_1(ascjson* self, char* vargv) {
  return self->count_names(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_aliases_1(ascjson* self, char* vargv) {
  return self->aliases(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_isas_1(ascjson* self, char* vargv) {
  return self->isas(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_cliques_1(ascjson* self, char* vargv) {
  return self->cliques(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_brow_child_list_1(ascjson* self, char* vargv) {
  return self->brow_child_list(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_bmerge_1(ascjson* self, char* vargv) {
  return self->bmerge(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_brefine_1(ascjson* self, char* vargv) {
  return self->brefine(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_bmakealike_1(ascjson* self, char* vargv) {
  return self->bmakealike(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_b_isplottable_1(ascjson* self, char* vargv) {
  return self->b_isplottable(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_b_prepplotfile_1(ascjson* self, char* vargv) {
  return self->b_prepplotfile(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_free_all_vars_1(ascjson* self, char* vargv) {
  return self->free_all_vars(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_Asc_SimBinTokenSetOptionsHC_1(ascjson* self, char* vargv) {
  return self->Asc_SimBinTokenSetOptionsHC(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_sims_1(ascjson* self, char* vargv) {
  return self->sims(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_sim_instantiate_1(ascjson* self, char* vargv) {
  return self->sim_instantiate(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_sim_unique_1(ascjson* self, char* vargv) {
  return self->sim_unique(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_sim_reinstantiate_1(ascjson* self, char* vargv) {
  return self->sim_reinstantiate(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__sims_copy_1(ascjson* self, char* vargv) {
  return self->x__sims_copy(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__sims_proto_1(ascjson* self, char* vargv) {
  return self->x__sims_proto(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__sims_saveinst_1(ascjson* self, char* vargv) {
  return self->x__sims_saveinst(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_sim_destroy_1(ascjson* self, char* vargv) {
  return self->sim_destroy(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_simlistpending_1(ascjson* self, char* vargv) {
  return self->simlistpending(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_ddefine_1(ascjson* self, char* vargv) {
  return self->ddefine(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_ddiffdefine_1(ascjson* self, char* vargv) {
  return self->ddiffdefine(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_disp_1(ascjson* self, char* vargv) {
  return self->disp(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_hier_1(ascjson* self, char* vargv) {
  return self->hier(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_file_by_type_1(ascjson* self, char* vargv) {
  return self->file_by_type(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dchild_1(ascjson* self, char* vargv) {
  return self->dchild(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_drefines_me_1(ascjson* self, char* vargv) {
  return self->drefines_me(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_drefines_meall_1(ascjson* self, char* vargv) {
  return self->drefines_meall(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_drefinement_tree_1(ascjson* self, char* vargv) {
  return self->drefinement_tree(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dgetparts_1(ascjson* self, char* vargv) {
  return self->dgetparts(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_disroot_type_1(ascjson* self, char* vargv) {
  return self->disroot_type(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__probe_1(ascjson* self, char* vargv) {
  return self->x__probe(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__var_analyze_1(ascjson* self, char* vargv) {
  return self->x__var_analyze(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__rel_analyze_1(ascjson* self, char* vargv) {
  return self->x__rel_analyze(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__userdata_init_1(ascjson* self, char* vargv) {
  return self->x__userdata_init(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__userdata_create_1(ascjson* self, char* vargv) {
  return self->x__userdata_create(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__userdata_destroy_1(ascjson* self, char* vargv) {
  return self->x__userdata_destroy(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__userdata_save_1(ascjson* self, char* vargv) {
  return self->x__userdata_save(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__userdata_restore_1(ascjson* self, char* vargv) {
  return self->x__userdata_restore(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__userdata_query_1(ascjson* self, char* vargv) {
  return self->x__userdata_query(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__userdata_print_1(ascjson* self, char* vargv) {
  return self->x__userdata_print(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_get_model_children_1(ascjson* self, char* vargv) {
  return self->get_model_children(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_checksim_1(ascjson* self, char* vargv) {
  return self->slv_checksim(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_checksys_1(ascjson* self, char* vargv) {
  return self->slv_checksys(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_get_obj_list_1(ascjson* self, char* vargv) {
  return self->slv_get_obj_list(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_set_obj_by_num_1(ascjson* self, char* vargv) {
  return self->slv_set_obj_by_num(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_get_obj_num_1(ascjson* self, char* vargv) {
  return self->slv_get_obj_num(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_get_parms_1(ascjson* self, char* vargv) {
  return self->slv_get_parms(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_set_slv_parms_1(ascjson* self, char* vargv) {
  return self->set_slv_parms(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_set_slv_parmsnew_1(ascjson* self, char* vargv) {
  return self->set_slv_parmsnew(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_get_parmsnew_1(ascjson* self, char* vargv) {
  return self->slv_get_parmsnew(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_get_insttype_1(ascjson* self, char* vargv) {
  return self->slv_get_insttype(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_get_cost_page_1(ascjson* self, char* vargv) {
  return self->slv_get_cost_page(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_get_stat_page_1(ascjson* self, char* vargv) {
  return self->slv_get_stat_page(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_get_objval_1(ascjson* self, char* vargv) {
  return self->slv_get_objval(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_get_instname_1(ascjson* self, char* vargv) {
  return self->slv_get_instname(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_get_pathname_1(ascjson* self, char* vargv) {
  return self->slv_get_pathname(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_get_vr_1(ascjson* self, char* vargv) {
  return self->slv_get_vr(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slvdump_1(ascjson* self, char* vargv) {
  return self->slvdump(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_presolve_1(ascjson* self, char* vargv) {
  return self->slv_presolve(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_reanalyze_1(ascjson* self, char* vargv) {
  return self->slv_reanalyze(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_check_and_reanalyze_1(ascjson* self, char* vargv) {
  return self->slv_check_and_reanalyze(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_set_independent_1(ascjson* self, char* vargv) {
  return self->slv_set_independent(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_resolve_1(ascjson* self, char* vargv) {
  return self->slv_resolve(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_solve_1(ascjson* self, char* vargv) {
  return self->slv_solve(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_iterate_1(ascjson* self, char* vargv) {
  return self->slv_iterate(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_available_1(ascjson* self, char* vargv) {
  return self->slv_available(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_number_1(ascjson* self, char* vargv) {
  return self->slv_number(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_name_1(ascjson* self, char* vargv) {
  return self->slv_name(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_linsol_names_1(ascjson* self, char* vargv) {
  return self->slv_linsol_names(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_eligible_solver_1(ascjson* self, char* vargv) {
  return self->slv_eligible_solver(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_select_solver_1(ascjson* self, char* vargv) {
  return self->slv_select_solver(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_get_solver_1(ascjson* self, char* vargv) {
  return self->slv_get_solver(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_flush_solver_1(ascjson* self, char* vargv) {
  return self->slv_flush_solver(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_import_qlfdid_1(ascjson* self, char* vargv) {
  return self->slv_import_qlfdid(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_lnmget_1(ascjson* self, char* vargv) {
  return self->slv_lnmget(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_lnmset_1(ascjson* self, char* vargv) {
  return self->slv_lnmset(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_set_haltflag_1(ascjson* self, char* vargv) {
  return self->slv_set_haltflag(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slvhelp_1(ascjson* self, char* vargv) {
  return self->slvhelp(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_near_bounds_1(ascjson* self, char* vargv) {
  return self->slv_near_bounds(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_far_from_nominals_1(ascjson* self, char* vargv) {
  return self->slv_far_from_nominals(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_slv_monitor_1(ascjson* self, char* vargv) {
  return self->slv_monitor(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_get_blk_of_var_1(ascjson* self, char* vargv) {
  return self->dbg_get_blk_of_var(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_get_blk_of_eqn_1(ascjson* self, char* vargv) {
  return self->dbg_get_blk_of_eqn(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_get_blk_coords_1(ascjson* self, char* vargv) {
  return self->dbg_get_blk_coords(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_get_eqn_of_var_1(ascjson* self, char* vargv) {
  return self->dbg_get_eqn_of_var(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_get_varpartition_1(ascjson* self, char* vargv) {
  return self->dbg_get_varpartition(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_get_eqnpartition_1(ascjson* self, char* vargv) {
  return self->dbg_get_eqnpartition(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_list_rels_1(ascjson* self, char* vargv) {
  return self->dbg_list_rels(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_list_vars_1(ascjson* self, char* vargv) {
  return self->dbg_list_vars(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_write_var_1(ascjson* self, char* vargv) {
  return self->dbg_write_var(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_write_unattvar_1(ascjson* self, char* vargv) {
  return self->dbg_write_unattvar(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_brow_write_var_1(ascjson* self, char* vargv) {
  return self->brow_write_var(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_write_rel_1(ascjson* self, char* vargv) {
  return self->dbg_write_rel(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_brow_write_rel_1(ascjson* self, char* vargv) {
  return self->brow_write_rel(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_write_obj_1(ascjson* self, char* vargv) {
  return self->dbg_write_obj(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_brow_write_obj_1(ascjson* self, char* vargv) {
  return self->brow_write_obj(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_write_varattr_1(ascjson* self, char* vargv) {
  return self->dbg_write_varattr(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_write_qlfattr_1(ascjson* self, char* vargv) {
  return self->dbg_write_qlfattr(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_rel_included_1(ascjson* self, char* vargv) {
  return self->dbg_rel_included(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_var_fixed_1(ascjson* self, char* vargv) {
  return self->dbg_var_fixed(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_get_incidence_1(ascjson* self, char* vargv) {
  return self->dbg_get_incidence(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_get_order_1(ascjson* self, char* vargv) {
  return self->dbg_get_order(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_write_incidence_1(ascjson* self, char* vargv) {
  return self->dbg_write_incidence(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_find_eligible_1(ascjson* self, char* vargv) {
  return self->dbg_find_eligible(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_brow_find_eligible_1(ascjson* self, char* vargv) {
  return self->brow_find_eligible(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_consistency_analysis_1(ascjson* self, char* vargv) {
  return self->dbg_consistency_analysis(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_global_eligible_1(ascjson* self, char* vargv) {
  return self->dbg_global_eligible(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_find_activerels_1(ascjson* self, char* vargv) {
  return self->dbg_find_activerels(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_brow_find_activerels_1(ascjson* self, char* vargv) {
  return self->brow_find_activerels(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_struct_singular_1(ascjson* self, char* vargv) {
  return self->dbg_struct_singular(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_num_block_singular_1(ascjson* self, char* vargv) {
  return self->dbg_num_block_singular(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_var_free2nom_1(ascjson* self, char* vargv) {
  return self->var_free2nom(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_var_nom2free_1(ascjson* self, char* vargv) {
  return self->var_nom2free(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_calc_relnoms_1(ascjson* self, char* vargv) {
  return self->dbg_calc_relnoms(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_check_rels_1(ascjson* self, char* vargv) {
  return self->dbg_check_rels(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_write_slv0_xsys_1(ascjson* self, char* vargv) {
  return self->dbg_write_slv0_xsys(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_write_slv0_sys_1(ascjson* self, char* vargv) {
  return self->dbg_write_slv0_sys(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_mtxwriteplot_1(ascjson* self, char* vargv) {
  return self->dbg_mtxwriteplot(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbg_calc_jacobian_1(ascjson* self, char* vargv) {
  return self->dbg_calc_jacobian(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_dbghelp_1(ascjson* self, char* vargv) {
  return self->dbghelp(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_integrate_setup_1(ascjson* self, char* vargv) {
  return self->integrate_setup(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_integrate_cleanup_1(ascjson* self, char* vargv) {
  return self->integrate_cleanup(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_integrate_set_y_file_1(ascjson* self, char* vargv) {
  return self->integrate_set_y_file(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_integrate_set_obs_file_1(ascjson* self, char* vargv) {
  return self->integrate_set_obs_file(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_integrate_logunits_1(ascjson* self, char* vargv) {
  return self->integrate_logunits(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_integrate_logformat_1(ascjson* self, char* vargv) {
  return self->integrate_logformat(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_integrate_get_samples_1(ascjson* self, char* vargv) {
  return self->integrate_get_samples(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_integrate_set_samples_1(ascjson* self, char* vargv) {
  return self->integrate_set_samples(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_integrate_able_1(ascjson* self, char* vargv) {
  return self->integrate_able(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_x__mtx_norms_1(ascjson* self, char* vargv) {
  return self->x__mtx_norms(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_mtx_gui_plot_incidence_1(ascjson* self, char* vargv) {
  return self->mtx_gui_plot_incidence(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_mtxhelp_1(ascjson* self, char* vargv) {
  return self->mtxhelp(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_destroy_units_1(ascjson* self, char* vargv) {
  return self->u_destroy_units(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_setSIdef_1(ascjson* self, char* vargv) {
  return self->u_setSIdef(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_getbasedef_1(ascjson* self, char* vargv) {
  return self->u_getbasedef(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_dump_1(ascjson* self, char* vargv) {
  return self->u_dump(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_dims_1(ascjson* self, char* vargv) {
  return self->u_dims(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_dim_setverify_1(ascjson* self, char* vargv) {
  return self->u_dim_setverify(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_dim2num_1(ascjson* self, char* vargv) {
  return self->u_dim2num(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_num2dim_1(ascjson* self, char* vargv) {
  return self->u_num2dim(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_frombasedim_1(ascjson* self, char* vargv) {
  return self->u_frombasedim(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_fromatomdim_1(ascjson* self, char* vargv) {
  return self->u_fromatomdim(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_getdimatoms_1(ascjson* self, char* vargv) {
  return self->u_getdimatoms(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_change_baseunit_1(ascjson* self, char* vargv) {
  return self->u_change_baseunit(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_getprec_1(ascjson* self, char* vargv) {
  return self->u_getprec(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_setprec_1(ascjson* self, char* vargv) {
  return self->u_setprec(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_get_atoms_1(ascjson* self, char* vargv) {
  return self->u_get_atoms(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_get_units_1(ascjson* self, char* vargv) {
  return self->u_get_units(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_set_user_1(ascjson* self, char* vargv) {
  return self->u_set_user(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_get_user_1(ascjson* self, char* vargv) {
  return self->u_get_user(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_get_list_1(ascjson* self, char* vargv) {
  return self->u_get_list(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_clear_user_1(ascjson* self, char* vargv) {
  return self->u_clear_user(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_getval_1(ascjson* self, char* vargv) {
  return self->u_getval(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_browgetval_1(ascjson* self, char* vargv) {
  return self->u_browgetval(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_slvgetrelval_1(ascjson* self, char* vargv) {
  return self->u_slvgetrelval(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_slvgetvarval_1(ascjson* self, char* vargv) {
  return self->u_slvgetvarval(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_u_slvgetobjval_1(ascjson* self, char* vargv) {
  return self->u_slvgetobjval(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_uhelp_1(ascjson* self, char* vargv) {
  return self->uhelp(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_srefine_1(ascjson* self, char* vargv) {
  return self->srefine(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_smerge_1(ascjson* self, char* vargv) {
  return self->smerge(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_ascloadwin_1(ascjson* self, char* vargv) {
  return self->ascloadwin(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_gnutext_1(ascjson* self, char* vargv) {
  return self->gnutext(vargv);
}

const rt* EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson_help_1(ascjson* self, char* vargv) {
  return self->help(vargv);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ascjson___destroy___0(ascjson* self) {
  delete self;
}

// $vtype
vtype EMSCRIPTEN_KEEPALIVE emscripten_enum_vtype_SVjson() {
  return SVjson;
}
vtype EMSCRIPTEN_KEEPALIVE emscripten_enum_vtype_SVjson5() {
  return SVjson5;
}
vtype EMSCRIPTEN_KEEPALIVE emscripten_enum_vtype_SVcstr() {
  return SVcstr;
}

}

